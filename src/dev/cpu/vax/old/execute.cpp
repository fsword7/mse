/*
 * execute.cpp
 *
 *  Created on: July 21, 2018
 *      Author: Timothy Stark
 */

#include "emu/emucore.h"
#include "dev/cpu/vax/mtpr.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/fpu.h"
#include "dev/cpu/vax/cvax.h"
#include "dev/cpu/vax/opcodes.h"

// Register validation check
#define Validate(rn, max) if ((rn) >= (max)) throw RSVD_ADDR_FAULT;

//#define Validate(rn, max) \
//	if ((rn) >= (max)) {          \
//		printf("%s: (BAD!!) reg=%d (max %d) idx=%d spec=%02X\n", deviceName(), \
//			rn, max, idx, spec);  \
//		throw RSVD_ADDR_FAULT;    \
//	}

static const char *stopNames[] =
{
	"HALT Instruction"
};

static const char *ferrCodes[] =
{
	"Successful",
	"Overflow Error",
	"Underflow Error",
	"Divide by Zero",
	"Reserved Operand Fault"
};

void vax_cpu_base::execute() noexcept(false)
{
	int32_t  brDisp;
	bool     ovflg;
	uint32_t sts, flg;
	dopc_t   *dopc;
	int32_t  src1, src2, carry;
	int32_t  dst1, dst2;
	int32_t  src, dst, tmp;
	int64_t  srcq, srcq1, srcq2;
	int64_t  dstq, dstq1, dstq2;
	uint32_t usrc, usrc1, usrc2;
	uint32_t udst, utmp;
	uint32_t mask;
	int32_t  cnt;
	int32_t  entry, pred, succ;
	int32_t  prev, next;
	int32_t  queue, head, tail;

	int32_t  srcx[4],  dstx[4];
	uint32_t usrcx[4], udstx[4];
	uint32_t uresx[4];
	uint32_t uidx[4];

//	register int64_t  srcqx[4],  dstqx[4];
//	register uint64_t usrcqx[4], udstqx[4];

	// Reset instruction steam
	flushvi();

	state = cpuRunning;

	while (1) {
		try {
			if (state == cpuStopping)
				throw STOP_OPR;

			// Check any pending SCB code
			if (scbCode != -1) {
				fault(scbCode);
				scbCode = -1;
			}
			faultAddr = REG_PC;

			// Check trap and interrupt requests first
			if (irqFlags != 0) {
				interrupt();
				UpdateIRQ();
				continue;
			}

			if (psReg & PSL_TP) {
				psReg &= ~PSL_TP;
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_EXCEPTION))
					dbg.log("%s: (EXC) Trace Trap at PC %08X\n", deviceName(), REG_PC);
#endif /* ENABLE_DEBUG */
				exception(IE_EXC, SCB_TP, 0);
				continue;
			}
			if (psReg & PSW_T)
				psReg |= PSL_TP;

#ifdef ENABLE_DEBUG
//			if (dbg.checkFlags(DBG_TRACE))
//				disassmeble(nullptr, faultAddr);
#endif /* ENABLE_DEBUG */

			// Fetch instruction from current stream
			opCode = uint8_t(readvi(LN_BYTE));
			if (opCode > OPC_nEXTEND) {
				opCode  = (opCode - OPC_nEXTEND) << 8;
				opCode |= uint8_t(readvi(LN_BYTE));
			}


			// Decode operands
			dopc = &dopCode[opCode];
			if (dopc->flag & OPF_RSVD)
				throw RSVD_INST_FAULT;

			if (psReg & PSL_FPD) {
				if ((dopc->flag & OPF_FPD) == 0)
					throw RSVD_INST_FAULT;
			} else {
				rqCount = 0;
				for (int idx = 0, opidx = 0; idx < dopc->nModes; idx++) {
					int opr = dopc->opMode[idx];
					int rn, ireg, spec;
					uint32_t mAddr, off;

					switch (opr) {
						case BB: // Byte Branch Mode
							opReg[opidx++] = SXTB(readvi(LN_BYTE));
							continue;

						case BW: // Word Branch Mode
							opReg[opidx++] = SXTW(readvi(LN_WORD));
							continue;

						default:
							spec  = readvi(LN_BYTE);
							rn    = spec & 0xF;
							opr  |= (spec & 0xF0);
					}

	//				printf("%s: OP %d %02X => R%d (%04X)\n", deviceName(),
	//					idx, spec, rn, opr);

					switch(opr) {
						// Short Literal Address Mode
						//   Only read access is allowed.
						case LIT0|RB: case LIT0|RW: case LIT0|RL:
						case LIT1|RB: case LIT1|RW: case LIT1|RL:
						case LIT2|RB: case LIT2|RW: case LIT2|RL:
						case LIT3|RB: case LIT3|RW: case LIT3|RL:
							opReg[opidx++] = spec;
							continue;
						case LIT0|RQ: case LIT1|RQ: case LIT2|RQ: case LIT3|RQ:
							opReg[opidx++] = spec;
							opReg[opidx++] = 0;
							continue;
						case LIT0|RO: case LIT1|RO: case LIT2|RO: case LIT3|RO:
							opReg[opidx++] = spec;
							opReg[opidx++] = 0;
							opReg[opidx++] = 0;
							opReg[opidx++] = 0;
							continue;

						case LIT0|RF: case LIT1|RF: case LIT2|RF: case LIT3|RF:
							opReg[opidx++] = (spec << 4) | 0x4000;
							continue;
						case LIT0|RD: case LIT1|RD: case LIT2|RD: case LIT3|RD:
							opReg[opidx++] = (spec << 4) | 0x4000;
							opReg[opidx++] = 0;
							continue;
						case LIT0|RG: case LIT1|RG: case LIT2|RG: case LIT3|RG:
							opReg[opidx++] = (spec << 1) | 0x4000;
							opReg[opidx++] = 0;
							continue;
						case LIT0|RH: case LIT1|RH: case LIT2|RH: case LIT3|RH:
							opReg[opidx++] =
								((spec & 7) << 29) | (0x4000 | ((spec >> 3) & 7));
							opReg[opidx++] = 0;
							opReg[opidx++] = 0;
							opReg[opidx++] = 0;
							continue;


						// Register Address Mode
						// Read Access for General Registers
						case REG|RB:
							Validate(rn, REG_nPC);
							opReg[opidx++] = CPU_REGUB(rn);
							continue;
						case REG|RW:
							Validate(rn, REG_nPC);
							opReg[opidx++] = CPU_REGUW(rn);
							continue;
						case REG|RL: case REG|RF:
							Validate(rn, REG_nPC);
							opReg[opidx++] = CPU_REGUL(rn);
							continue;
						case REG|RQ: case REG|RD: case REG|RG:
							Validate(rn, REG_nSP);
							opReg[opidx++] = CPU_REGUL(rn);
							opReg[opidx++] = CPU_REGUL(rn+1);
							continue;
						case REG|RO: case REG|RH:
							Validate(rn, REG_nAP);
							opReg[opidx++] = CPU_REGUL(rn);
							opReg[opidx++] = CPU_REGUL(rn+1);
							opReg[opidx++] = CPU_REGUL(rn+2);
							opReg[opidx++] = CPU_REGUL(rn+3);
							continue;

						// Modify Access for General Registers
						case REG|MB:
							Validate(rn, REG_nPC);
							opReg[opidx++] = CPU_REGUB(rn);
							opReg[opidx++] = ~rn;
							continue;
						case REG|MW:
							Validate(rn, REG_nPC);
							opReg[opidx++] = CPU_REGUW(rn);
							opReg[opidx++] = ~rn;
							continue;
						case REG|ML:
							Validate(rn, REG_nPC);
							opReg[opidx++] = CPU_REGUL(rn);
							opReg[opidx++] = ~rn;
							continue;
						case REG|MQ:
							Validate(rn, REG_nSP);
							opReg[opidx++] = CPU_REGUL(rn);
							opReg[opidx++] = CPU_REGUL(rn+1);
							opReg[opidx++] = ~rn;
							continue;
						case REG|MO:
							Validate(rn, REG_nAP);
							opReg[opidx++] = CPU_REGUL(rn);
							opReg[opidx++] = CPU_REGUL(rn+1);
							opReg[opidx++] = CPU_REGUL(rn+2);
							opReg[opidx++] = CPU_REGUL(rn+3);
							opReg[opidx++] = ~rn;
							continue;

						// Write Access for General Registers
						case REG|WB: case REG|WW: case REG|WL: case REG|WQ:
						case REG|WO: case REG|VB:
							Validate(rn, REG_nPC);
							opReg[opidx++] = ~rn;
							continue;


						// Register Deferred Address Mode (Rn)
						case REGD|RB: case REGD|RW: case REGD|RL: case REGD|RF:
							Validate(rn, REG_nPC);
							opReg[opidx++] = readv(CPU_REGUL(rn), OPM_SIZE(opr), RACC);
							continue;
						case REGD|RQ: case REGD|RD: case REGD|RG:
							Validate(rn, REG_nPC);
							opReg[opidx++] = readv(CPU_REGUL(rn),   LN_LONG, RACC);
							opReg[opidx++] = readv(CPU_REGUL(rn)+4, LN_LONG, RACC);
							continue;
						case REGD|RO: case REGD|RH:
							Validate(rn, REG_nPC);
							opReg[opidx++] = readv(CPU_REGUL(rn),    LN_LONG, RACC);
							opReg[opidx++] = readv(CPU_REGUL(rn)+4,  LN_LONG, RACC);
							opReg[opidx++] = readv(CPU_REGUL(rn)+8,  LN_LONG, RACC);
							opReg[opidx++] = readv(CPU_REGUL(rn)+12, LN_LONG, RACC);
							continue;

						case REGD|MB: case REGD|MW: case REGD|ML:
							Validate(rn, REG_nPC);
							opReg[opidx++] = readv(CPU_REGUL(rn), OPM_SIZE(opr), WACC);
							opReg[opidx++] = CPU_REGUL(rn);
							continue;
						case REGD|MQ:
							Validate(rn, REG_nPC);
							opReg[opidx++] = readv(CPU_REGUL(rn),   LN_LONG, WACC);
							opReg[opidx++] = readv(CPU_REGUL(rn)+4, LN_LONG, WACC);
							opReg[opidx++] = CPU_REGUL(rn);
							continue;
						case REGD|MO:
							Validate(rn, REG_nPC);
							opReg[opidx++] = readv(CPU_REGUL(rn),    LN_LONG, WACC);
							opReg[opidx++] = readv(CPU_REGUL(rn)+4,  LN_LONG, WACC);
							opReg[opidx++] = readv(CPU_REGUL(rn)+8,  LN_LONG, WACC);
							opReg[opidx++] = readv(CPU_REGUL(rn)+12, LN_LONG, WACC);
							opReg[opidx++] = CPU_REGUL(rn);
							continue;

						case REGD|WB: case REGD|WW: case REGD|WL: case REGD|WQ:
						case REGD|AB: case REGD|AW: case REGD|AL: case REGD|AQ:
						case REGD|WO: case REGD|AO: case REGD|VB:
							Validate(rn, REG_nPC);
							opReg[opidx++] = CPU_REGUL(rn);
							continue;


						// Autodecrement Address Mode -(Rn)
						case ADEC|RB: case ADEC|RW: case ADEC|RL: case ADEC|RF:
							Validate(rn, REG_nPC);
							gpReg[rn].l     -= OPM_SIZE(opr);
							rqReg[rqCount++] = (OPM_SIZE(opr) << 4) | rn;
							opReg[opidx++]   = readv(CPU_REGUL(rn), OPM_SIZE(opr), RACC);
							continue;
						case ADEC|RQ: case ADEC|RD: case ADEC|RG:
							Validate(rn, REG_nPC);
							gpReg[rn].l     -= LN_QUAD;
							rqReg[rqCount++] = (LN_QUAD << 4) | rn;
							opReg[opidx++]   = readv(CPU_REGUL(rn),   LN_LONG, RACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+4, LN_LONG, RACC);
							continue;
						case ADEC|RO: case ADEC|RH:
							Validate(rn, REG_nPC);
							gpReg[rn].l     -= LN_OCTA;
							rqReg[rqCount++] = (LN_OCTA << 4) | rn;
							opReg[opidx++]   = readv(CPU_REGUL(rn),    LN_LONG, RACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+4,  LN_LONG, RACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+8,  LN_LONG, RACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+12, LN_LONG, RACC);
							continue;

						case ADEC|MB: case ADEC|MW: case ADEC|ML:
							Validate(rn, REG_nPC);
							gpReg[rn].l     -= OPM_SIZE(opr);
							rqReg[rqCount++] = (OPM_SIZE(opr) << 4) | rn;
							opReg[opidx++]   = readv(CPU_REGUL(rn), OPM_SIZE(opr), WACC);
							opReg[opidx++]   = CPU_REGUL(rn);
							continue;
						case ADEC|MQ:
							Validate(rn, REG_nPC);
							gpReg[rn].l     -= LN_QUAD;
							rqReg[rqCount++] = (LN_QUAD << 4) | rn;
							opReg[opidx++]   = readv(CPU_REGUL(rn),   LN_LONG, WACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+4, LN_LONG, WACC);
							opReg[opidx++]   = CPU_REGUL(rn);
							continue;
						case ADEC|MO:
							Validate(rn, REG_nPC);
							gpReg[rn].l     -= LN_OCTA;
							rqReg[rqCount++] = (LN_OCTA << 4) | rn;
							opReg[opidx++]   = readv(CPU_REGUL(rn),    LN_LONG, WACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+4,  LN_LONG, WACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+8,  LN_LONG, WACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+12, LN_LONG, WACC);
							opReg[opidx++]   = CPU_REGUL(rn);
							continue;

						case ADEC|WB: case ADEC|WW: case ADEC|WL: case ADEC|WQ:
						case ADEC|AB: case ADEC|AW: case ADEC|AL: case ADEC|AQ:
						case ADEC|WO: case ADEC|AO: case ADEC|VB:
							Validate(rn, REG_nPC);
							gpReg[rn].l     -= OPM_SIZE(opr);
							rqReg[rqCount++] = (OPM_SIZE(opr) << 4) | rn;
							opReg[opidx++]   = CPU_REGUL(rn);
							continue;

						// Autoincrement/Immediate Address Mode
						case AINC|RB: case AINC|RW: case AINC|RL: case AINC|RF:
							if (rn < REG_nPC) {
								opReg[opidx++]   = readv(CPU_REGUL(rn), OPM_SIZE(opr), RACC);
								rqReg[rqCount++] = (-OPM_SIZE(opr) << 4) | rn;
								gpReg[rn].l     += OPM_SIZE(opr);
							} else
								opReg[opidx++] = readvi(OPM_SIZE(opr));
							continue;

						case AINC|RQ: case AINC|RD: case AINC|RG:
							if (rn < REG_nPC) {
								opReg[opidx++]   = readv(CPU_REGUL(rn),   LN_LONG, RACC);
								opReg[opidx++]   = readv(CPU_REGUL(rn)+4, LN_LONG, RACC);
								rqReg[rqCount++] = (-LN_QUAD << 4) | rn;
								gpReg[rn].l     += LN_QUAD;
							} else {
								opReg[opidx++] = readvi(LN_LONG);
								opReg[opidx++] = readvi(LN_LONG);
							}
							continue;

						case AINC|RO: case AINC|RH:
							if (rn < REG_nPC) {
								opReg[opidx++]   = readv(CPU_REGUL(rn),    LN_LONG, RACC);
								opReg[opidx++]   = readv(CPU_REGUL(rn)+4,  LN_LONG, RACC);
								opReg[opidx++]   = readv(CPU_REGUL(rn)+8,  LN_LONG, RACC);
								opReg[opidx++]   = readv(CPU_REGUL(rn)+12, LN_LONG, RACC);
								rqReg[rqCount++] = (-LN_OCTA << 4) | rn;
								gpReg[rn].l     += LN_OCTA;
							} else {
								opReg[opidx++] = readvi(LN_LONG);
								opReg[opidx++] = readvi(LN_LONG);
								opReg[opidx++] = readvi(LN_LONG);
								opReg[opidx++] = readvi(LN_LONG);
							}
							continue;

						case AINC|MB: case AINC|MW: case AINC|ML:
							Validate(rn, REG_nPC);
							opReg[opidx++]   = readv(CPU_REGUL(rn), OPM_SIZE(opr), WACC);
							opReg[opidx++]   = CPU_REGUL(rn);
							rqReg[rqCount++] = (-OPM_SIZE(opr) << 4) | rn;
							gpReg[rn].l     += OPM_SIZE(opr);
							continue;

						case AINC|MQ:
							Validate(rn, REG_nPC);
							opReg[opidx++]   = readv(CPU_REGUL(rn),   LN_LONG, WACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+4, LN_LONG, WACC);
							opReg[opidx++]   = CPU_REGUL(rn);
							rqReg[rqCount++] = (-LN_QUAD << 4) | rn;
							gpReg[rn].l     += LN_QUAD;
							continue;

						case AINC|MO:
							Validate(rn, REG_nPC);
							opReg[opidx++]   = readv(CPU_REGUL(rn),    LN_LONG, WACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+4,  LN_LONG, WACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+8,  LN_LONG, WACC);
							opReg[opidx++]   = readv(CPU_REGUL(rn)+12, LN_LONG, WACC);
							opReg[opidx++]   = CPU_REGUL(rn);
							rqReg[rqCount++] = (-LN_OCTA << 4) | rn;
							gpReg[rn].l     += LN_OCTA;
							continue;

						case AINC|WB: case AINC|WW: case AINC|WL:
						case AINC|AB: case AINC|AW: case AINC|AL:
						case AINC|VB:
							opReg[opidx++] = CPU_REGUL(rn);
							if (rn < REG_nPC) {
								rqReg[rqCount++] = (-OPM_SIZE(opr) << 4) | rn;
								gpReg[rn].l     += OPM_SIZE(opr);
							} else
								readvi(OPM_SIZE(opr));
							continue;

						case AINC|WQ: case AINC|AQ:
							opReg[opidx++] = CPU_REGUL(rn);
							if (rn < REG_nPC) {
								rqReg[rqCount++] = (-OPM_SIZE(opr) << 4) | rn;
								gpReg[rn].l     += OPM_SIZE(opr);
							} else {
								readvi(LN_LONG);
								readvi(LN_LONG);
							}
							continue;

						case AINC|WO: case AINC|AO:
							opReg[opidx++] = CPU_REGUL(rn);
							if (rn < REG_nPC) {
								rqReg[rqCount++] = (-OPM_SIZE(opr) << 4) | rn;
								gpReg[rn].l     += OPM_SIZE(opr);
							} else {
								readvi(LN_LONG);
								readvi(LN_LONG);
								readvi(LN_LONG);
								readvi(LN_LONG);
							}
							continue;

						// Autoincrement Deferred Address Mode
						case AINCD|RB: case AINCD|RW: case AINCD|RL: case AINCD|RF:
							if (rn < REG_nPC) {
								mAddr            = readv(CPU_REGUL(rn), LN_LONG, RACC);
								rqReg[rqCount++] = (-LN_LONG << 4) | rn;
								gpReg[rn].l     += LN_LONG;
							} else
								mAddr = readvi(LN_LONG);
							opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), RACC);
							continue;

						case AINCD|RQ: case AINCD|RD: case AINCD|RG:
							if (rn < REG_nPC) {
								mAddr            = readv(CPU_REGUL(rn), LN_LONG, RACC);
								rqReg[rqCount++] = (-LN_LONG << 4) | rn;
								gpReg[rn].l     += LN_LONG;
							} else
								mAddr = readvi(LN_LONG);
							opReg[opidx++] = readv(mAddr,   LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+4, LN_LONG, RACC);
							continue;

						case AINCD|RO: case AINCD|RH:
							if (rn < REG_nPC) {
								mAddr            = readv(CPU_REGUL(rn), LN_LONG, RACC);
								rqReg[rqCount++] = (-LN_LONG << 4) | rn;
								gpReg[rn].l     += LN_LONG;
							} else
								mAddr = readvi(LN_LONG);
							opReg[opidx++] = readv(mAddr,    LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+4,  LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+8,  LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+12, LN_LONG, RACC);
							continue;

						case AINCD|MB: case AINCD|MW: case AINCD|ML:
							if (rn < REG_nPC) {
								mAddr            = readv(CPU_REGUL(rn), LN_LONG, RACC);
								rqReg[rqCount++] = (-LN_LONG << 4) | rn;
								gpReg[rn].l     += LN_LONG;
							} else
								mAddr = readvi(LN_LONG);
							opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), WACC);
							opReg[opidx++] = mAddr;
							continue;

						case AINCD|MQ:
							if (rn < REG_nPC) {
								mAddr            = readv(CPU_REGUL(rn), LN_LONG, RACC);
								rqReg[rqCount++] = (-LN_LONG << 4) | rn;
								gpReg[rn].l     += LN_LONG;
							} else
								mAddr = readvi(LN_LONG);
							opReg[opidx++] = readv(mAddr,   LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+4, LN_LONG, WACC);
							opReg[opidx++] = mAddr;
							continue;

						case AINCD|MO:
							if (rn < REG_nPC) {
								mAddr            = readv(CPU_REGUL(rn), LN_LONG, RACC);
								rqReg[rqCount++] = (-LN_LONG << 4) | rn;
								gpReg[rn].l     += LN_LONG;
							} else
								mAddr = readvi(LN_LONG);
							opReg[opidx++] = readv(mAddr,    LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+4,  LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+8,  LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+12, LN_LONG, WACC);
							opReg[opidx++] = mAddr;
							continue;

						case AINCD|WB: case AINCD|WW: case AINCD|WL: case AINCD|WQ:
						case AINCD|AB: case AINCD|AW: case AINCD|AL: case AINCD|AQ:
						case AINCD|WO: case AINCD|AO: case AINCD|VB:
							if (rn < REG_nPC) {
								opReg[opidx++]   = readv(CPU_REGUL(rn), LN_LONG, RACC);
								rqReg[rqCount++] = (-LN_LONG << 4) | rn;
								gpReg[rn].l     += LN_LONG;
							} else
								opReg[opidx++] = readvi(LN_LONG);
							continue;


						// Byte-Displacement Address Mode
						case BDP|RB: case BDP|RW: case BDP|RL: case BDP|RF:
							off   = SXTB(readvi(LN_BYTE));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), RACC);
							continue;
						case BDP|RQ: case BDP|RG: case BDP|RD:
							off   = SXTB(readvi(LN_BYTE));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr,   LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+4, LN_LONG, RACC);
							continue;
						case BDP|RO: case BDP|RH:
							off   = SXTB(readvi(LN_BYTE));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr,    LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+4,  LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+8,  LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+12, LN_LONG, RACC);
							continue;

						case BDP|MB: case BDP|MW: case BDP|ML:
							off   = SXTB(readvi(LN_BYTE));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), WACC);
							opReg[opidx++] = mAddr;
							continue;
						case BDP|MQ:
							off   = SXTB(readvi(LN_BYTE));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr,   LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+4, LN_LONG, WACC);
							opReg[opidx++] = mAddr;
							continue;
						case BDP|MO:
							off   = SXTB(readvi(LN_BYTE));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr,    LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+4,  LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+8,  LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+12, LN_LONG, WACC);
							opReg[opidx++] = mAddr;
							continue;

						case BDP|WB: case BDP|WW: case BDP|WL: case BDP|WQ:
						case BDP|AB: case BDP|AW: case BDP|AL: case BDP|AQ:
						case BDP|WO: case BDP|AO: case BDP|VB:
							off   = SXTB(readvi(LN_BYTE));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = mAddr;
			//				PrintLog3(LOG_TRACE, NULL,
			//					"%s: (BDP) R%d => %08X + %08X => %08X\n",
			//						IO_DEVNAME(cpu), rn, CPU_REGUL(rn), off, mAddr);
							continue;

						// Byte-Displacement Deferred Address Mode
						case BDPD|RB: case BDPD|RW: case BDPD|RL: case BDPD|RF:
							off   = SXTB(readvi(LN_BYTE));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), RACC);
							continue;
						case BDPD|RQ: case BDPD|RG: case BDPD|RD:
							off   = SXTB(readvi(LN_BYTE));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr,   LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+4, LN_LONG, RACC);
							continue;
						case BDPD|RO: case BDPD|RH:
							off   = SXTB(readvi(LN_BYTE));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr,    LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+4,  LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+8,  LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+12, LN_LONG, RACC);
							continue;

						case BDPD|MB: case BDPD|MW: case BDPD|ML:
							off   = SXTB(readvi(LN_BYTE));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), WACC);
							opReg[opidx++] = mAddr;
							continue;
						case BDPD|MQ:
							off   = SXTB(readvi(LN_BYTE));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr,   LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+4, LN_LONG, WACC);
							opReg[opidx++] = mAddr;
							continue;
						case BDPD|MO:
							off   = SXTB(readvi(LN_BYTE));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr,    LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+4,  LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+8,  LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+12, LN_LONG, WACC);
							opReg[opidx++] = mAddr;
							continue;

						case BDPD|WB: case BDPD|WW: case BDPD|WL: case BDPD|WQ:
						case BDPD|AB: case BDPD|AW: case BDPD|AL: case BDPD|AQ:
						case BDPD|WO: case BDPD|AO: case BDPD|VB:
							off   = SXTB(readvi(LN_BYTE));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = mAddr;
							continue;

						// Word-Displacement Address Mode
						case WDP|RB: case WDP|RW: case WDP|RL: case WDP|RF:
							off   = SXTW(readvi(LN_WORD));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), RACC);
							continue;
						case WDP|RQ: case WDP|RG: case WDP|RD:
							off   = SXTW(readvi(LN_WORD));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr,   LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+4, LN_LONG, RACC);
							continue;
						case WDP|RO: case WDP|RH:
							off   = SXTW(readvi(LN_WORD));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr,    LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+4,  LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+8,  LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+12, LN_LONG, RACC);
							continue;

						case WDP|MB: case WDP|MW: case WDP|ML:
							off   = SXTW(readvi(LN_WORD));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), WACC);
							opReg[opidx++] = mAddr;
							continue;
						case WDP|MQ:
							off   = SXTW(readvi(LN_WORD));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr,   LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+4, LN_LONG, WACC);
							opReg[opidx++] = mAddr;
							continue;
						case WDP|MO:
							off   = SXTW(readvi(LN_WORD));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr,    LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+4,  LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+8,  LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+12, LN_LONG, WACC);
							opReg[opidx++] = mAddr;
							continue;

						case WDP|WB: case WDP|WW: case WDP|WL: case WDP|WQ:
						case WDP|AB: case WDP|AW: case WDP|AL: case WDP|AQ:
						case WDP|WO: case WDP|AO: case WDP|VB:
							off   = SXTW(readvi(LN_WORD));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = mAddr;
							continue;

						// Word-Displacement Deferred Address Mode
						case WDPD|RB: case WDPD|RW: case WDPD|RL: case WDPD|RF:
							off   = SXTW(readvi(LN_WORD));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), RACC);
							continue;
						case WDPD|RQ: case WDPD|RG: case WDPD|RD:
							off   = SXTW(readvi(LN_WORD));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr,   LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+4, LN_LONG, RACC);
							continue;
						case WDPD|RO: case WDPD|RH:
							off   = SXTW(readvi(LN_WORD));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr,    LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+4,  LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+8,  LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+12, LN_LONG, RACC);
							continue;

						case WDPD|MB: case WDPD|MW: case WDPD|ML:
							off   = SXTW(readvi(LN_WORD));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), WACC);
							opReg[opidx++] = mAddr;
							continue;
						case WDPD|MQ:
							off   = SXTW(readvi(LN_WORD));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr,   LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+4, LN_LONG, WACC);
							opReg[opidx++] = mAddr;
							continue;
						case WDPD|MO:
							off   = SXTW(readvi(LN_WORD));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr,    LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+4,  LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+8,  LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+12, LN_LONG, WACC);
							opReg[opidx++] = mAddr;
							continue;

						case WDPD|WB: case WDPD|WW: case WDPD|WL: case WDPD|WQ:
						case WDPD|AB: case WDPD|AW: case WDPD|AL: case WDPD|AQ:
						case WDPD|WO: case WDPD|AO: case WDPD|VB:
							off   = SXTW(readvi(LN_WORD));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = mAddr;
							continue;

						// Longword-Displacement Address Mode
						case LDP|RB: case LDP|RW: case LDP|RL: case LDP|RF:
							off   = SXTL(readvi(LN_LONG));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), RACC);
							continue;
						case LDP|RQ: case LDP|RG: case LDP|RD:
							off   = SXTL(readvi(LN_LONG));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr,   LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+4, LN_LONG, RACC);
							continue;
						case LDP|RO: case LDP|RH:
							off   = SXTL(readvi(LN_LONG));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr,    LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+4,  LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+8,  LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+12, LN_LONG, RACC);
							continue;

						case LDP|MB: case LDP|MW: case LDP|ML:
							off   = SXTL(readvi(LN_LONG));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), WACC);
							opReg[opidx++] = mAddr;
							continue;
						case LDP|MQ:
							off   = SXTL(readvi(LN_LONG));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr,   LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+4, LN_LONG, WACC);
							opReg[opidx++] = mAddr;
							continue;
						case LDP|MO:
							off   = SXTL(readvi(LN_LONG));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = readv(mAddr,    LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+4,  LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+8,  LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+12, LN_LONG, WACC);
							opReg[opidx++] = mAddr;
							continue;

						case LDP|WB: case LDP|WW: case LDP|WL: case LDP|WQ:
						case LDP|AB: case LDP|AW: case LDP|AL: case LDP|AQ:
						case LDP|WO: case LDP|AO: case LDP|VB:
							off   = SXTL(readvi(LN_LONG));
							mAddr = CPU_REGUL(rn) + off;
							opReg[opidx++] = mAddr;
							continue;

						// Longword-Displacement Deferred Address Mode
						case LDPD|RB: case LDPD|RW: case LDPD|RL: case LDPD|RF:
							off   = SXTL(readvi(LN_LONG));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), RACC);
							continue;
						case LDPD|RQ: case LDPD|RG: case LDPD|RD:
							off   = SXTL(readvi(LN_LONG));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr,   LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+4, LN_LONG, RACC);
							continue;
						case LDPD|RO: case LDPD|RH:
							off   = SXTL(readvi(LN_LONG));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr,    LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+4,  LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+8,  LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr+12, LN_LONG, RACC);
							continue;

						case LDPD|MB: case LDPD|MW: case LDPD|ML:
							off   = SXTL(readvi(LN_LONG));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr, OPM_SIZE(opr), WACC);
							opReg[opidx++] = mAddr;
							continue;
						case LDPD|MQ:
							off   = SXTL(readvi(LN_LONG));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr,   LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+4, LN_LONG, WACC);
							opReg[opidx++] = mAddr;
							continue;
						case LDPD|MO:
							off   = SXTL(readvi(LN_LONG));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = readv(mAddr,    LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+4,  LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+8,  LN_LONG, WACC);
							opReg[opidx++] = readv(mAddr+12, LN_LONG, WACC);
							opReg[opidx++] = mAddr;
							continue;

						case LDPD|WB: case LDPD|WW: case LDPD|WL: case LDPD|WQ:
						case LDPD|AB: case LDPD|AW: case LDPD|AL: case LDPD|AQ:
						case LDPD|WO: case LDPD|AO: case LDPD|VB:
							off   = SXTL(readvi(LN_LONG));
							mAddr = readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							opReg[opidx++] = mAddr;
							continue;

						// Indexed Register
						case IDX|RB: case IDX|RW: case IDX|RL: case IDX|RQ: case IDX|RO:
						case IDX|MB: case IDX|MW: case IDX|ML: case IDX|MQ: case IDX|MO:
						case IDX|WB: case IDX|WW: case IDX|WL: case IDX|WQ: case IDX|WO:
						case IDX|AB: case IDX|AW: case IDX|AL: case IDX|AQ: case IDX|AO:
						case IDX|RF: case IDX|RD: case IDX|RG: case IDX|RH: case IDX|VB:
							Validate(rn, REG_nPC);
							ireg = CPU_REGUL(rn) << OPM_SCALE(opr);
							spec = readvi(LN_BYTE);
							rn   = spec & 0xF;
							break;

						default:
							throw RSVD_ADDR_FAULT;
					}

			#ifdef DEBUG
			//		PrintLog3(LOG_TRACE, NULL,
			//			"%s: (OPR) Index %08X (Operand %02X)\n",
			//				IO_DEVNAME(cpu), idxReg, spec);
			#endif /* DEBUG */

					// Indexed Register
					switch(spec & 0xF0) {
						case REGD:
							Validate(rn, REG_nPC);
							ireg += CPU_REGUL(rn);
							break;

						case ADEC:
							Validate(rn, REG_nPC);
							rqReg[rqCount++] = (OPM_SIZE(opr) << 4) | rn;
							gpReg[rn].l     -= OPM_SIZE(opr);
							ireg            += CPU_REGUL(rn);
							break;

						case AINC:
							Validate(rn, REG_nPC);
							ireg            += CPU_REGUL(rn);
							rqReg[rqCount++] = (-OPM_SIZE(opr) << 4) | rn;
							gpReg[rn].l     += OPM_SIZE(opr);
							break;

						case AINCD:
							if (rn < REG_nPC) {
								ireg            += readv(CPU_REGUL(rn), LN_LONG, RACC);
								rqReg[rqCount++] = (-LN_LONG << 4) | rn;
								gpReg[rn].l     += LN_LONG;
							} else
								ireg += readvi(LN_LONG);
							break;

						case BDP:
							off     = SXTB(readvi(LN_BYTE));
							ireg += CPU_REGUL(rn) + off;
							break;

						case BDPD:
							off     = SXTB(readvi(LN_BYTE));
							ireg += readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							break;

						case WDP:
							off     = SXTW(readvi(LN_WORD));
							ireg += CPU_REGUL(rn) + off;
							break;

						case WDPD:
							off     = SXTW(readvi(LN_WORD));
							ireg += readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							break;

						case LDP:
							off     = SXTL(readvi(LN_LONG));
							ireg += CPU_REGUL(rn) + off;
							break;

						case LDPD:
							off     = SXTL(readvi(LN_LONG));
							ireg += readv(CPU_REGUL(rn) + off, LN_LONG, RACC);
							break;

						default:
							throw RSVD_ADDR_FAULT;
					}

					switch(opr & (OPM_ACC|OPM_LEN)) {
						case RB: case RW: case RL:
							opReg[opidx++] = readv(ireg, OPM_SIZE(opr), RACC);
							break;
						case RQ:
							opReg[opidx++] = readv(ireg,   LN_LONG, RACC);
							opReg[opidx++] = readv(ireg+4, LN_LONG, RACC);
							break;
						case RO:
							opReg[opidx++] = readv(ireg,    LN_LONG, RACC);
							opReg[opidx++] = readv(ireg+4,  LN_LONG, RACC);
							opReg[opidx++] = readv(ireg+8,  LN_LONG, RACC);
							opReg[opidx++] = readv(ireg+12, LN_LONG, RACC);
							break;

						case MB: case MW: case ML:
							opReg[opidx++] = readv(ireg, OPM_SIZE(opr), WACC);
							opReg[opidx++] = ireg;
							break;
						case MQ:
							opReg[opidx++] = readv(ireg,   LN_LONG, WACC);
							opReg[opidx++] = readv(ireg+4, LN_LONG, WACC);
							opReg[opidx++] = ireg;
							break;
						case MO:
							opReg[opidx++] = readv(ireg,    LN_LONG, WACC);
							opReg[opidx++] = readv(ireg+4,  LN_LONG, WACC);
							opReg[opidx++] = readv(ireg+8,  LN_LONG, WACC);
							opReg[opidx++] = readv(ireg+12, LN_LONG, WACC);
							opReg[opidx++] = ireg;
							break;

						case WB: case WW: case WL: case WQ: case WO:
						case AB: case AW: case AL: case AQ: case AO:
							opReg[opidx++] = ireg;
							break;
					}
				}
			}

			// Execute opcode
			switch (opCode) {

			case OPC_nHALT:
				// Must be in kernel mode
				if (PSL_GETCUR(psReg) != AM_KERNEL)
					throw PRIV_INST_FAULT;
//				halt(HALT_INST);
				throw STOP_HALT;

			case OPC_nNOP:
				// Do nothing...
				break;

			case OPC_nREI:
				resume();
				break;

			case OPC_nBPT:
				// Reset PC address
				REG_PC = faultAddr;
				exception(IE_EXC, SCB_BPT, 0);
				break;

			case OPC_nXFC:
				// Reset PC address
				REG_PC = faultAddr;
				exception(IE_EXC, SCB_XFC, 0);
				break;

			// INDEX - Index instruction
			case OPC_nINDEX:
				src  = SXTL(opReg[0]);
				src1 = SXTL(opReg[3]);
				src2 = SXTL(opReg[4]);
				if ((src < SXTL(opReg[1])) || (src > SXTL(opReg[2]))) {
					irqFlags &= ~IRQ_TRAP;
					irqFlags |= IRQ_SETTRAP(TRAP_SUBRNG);
				}

				dst = (src + src2) * src1;
				storel(opReg[5], dst);
				// Update condition codes
				SetNZ(ccReg, SXTL(dst), 0, 0);
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X (%08X to %08X) + %08X * %08X => %08X: %s\n", deviceName(),
						ZXTL(src), ZXTL(opReg[1]), ZXTL(opReg[2]), ZXTL(src2), ZXTL(src1),
						ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// BIxPSW instructions
			case OPC_nBICPSW:
				mask = uint16_t(opReg[0]);
				if (mask & PSW_MBZ)
					throw RSVD_OPND_FAULT;
				psReg &= ~mask;
				ccReg &= ~mask;
				break;
			case OPC_nBISPSW:
				mask = uint16_t(opReg[0]);
				if (mask & PSW_MBZ)
					throw RSVD_OPND_FAULT;
				psReg |= (mask & ~PSW_CC);
				ccReg |= (mask & PSW_CC);
				break;

			// MOVPSL instruction
			case OPC_nMOVPSL:
				dst = psReg | ccReg;
				storel(opReg[0], dst);
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: PSL %08X: %s\n", deviceName(),
						ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// MTPR/MFPR instructions
			case OPC_nMTPR:
				mtpr();
				break;
			case OPC_nMFPR:
				mfpr();
				break;

			// Bcc instructions - Branch On (condition)
			//
			// Opcodes: Condition
			//   12     Z EQL 0         BNEQ  Branch on Not Equal (signed)
			//   12     Z EQL 0         BNEQU Branch on Not Equal Unsigned
			//   13     Z EQL 1         BEQL  Branch on Equal (signed)
			//   13     Z EQL 1         BEQLU Branch on Equal Unsigned
			//   14     (N OR Z) EQL 0  BGTR  Branch on Greater Than (signed)
			//   15     (N OR Z) EQL 1  BLEQ  Branch on Less Than or Equal (signed)
			//   18     N EQL 0         BGEQ  Branch on Greater Than or Equal (signed)
			//   19     N EQL 1         BLSS  Branch on Less Than (signed)
			//   1A     (C OR Z) EQL 0  BGTRU Branch on Greater Than Unsigned
			//   1B     (C OR Z) EQL 1  BLEQU Branch on Less Than or Equal Unsigned
			//   1C     V EQL 0         BVC   Branch on Overflow Clear
			//   1D     V EQL 1         BVS   Branch on Overflow Set
			//   1E     C EQL 0         BGEQU Branch on Greater Than or Equal Unsigned
			//   1E     C EQL 0         BCC   Branch on Carry Clear
			//   1F     C EQL 1         BLSSU Branch on Less Than Unsigned
			//   1F     C EQL 1         BCS   Branch on Carry Set

			case OPC_nBNEQ:
				if ((ccReg & CC_Z) == 0) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBEQL:
				if (ccReg & CC_Z) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBGTR:
				if ((ccReg & (CC_N|CC_Z)) == 0) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBLEQ:
				if (ccReg & (CC_N|CC_Z)) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBGEQ:
				if ((ccReg & CC_N) == 0) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBLSS:
				if (ccReg & CC_N) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBGTRU:
				if ((ccReg & (CC_C|CC_Z)) == 0) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBLEQU:
				if (ccReg & (CC_C|CC_Z)) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBVC:
				if ((ccReg & CC_V) == 0) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBVS:
				if (ccReg & CC_V) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBCC:
				if ((ccReg & CC_C) == 0) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;

			case OPC_nBCS:
				if (ccReg & CC_C) {
					REG_PC += opReg[0];
					flushvi();
				}
				break;


			//  BRx instructions
			case OPC_nBRB:
				REG_PC += opReg[0];
				flushvi();
				break;
			case OPC_nBRW:
				REG_PC += opReg[0];
				flushvi();
				break;

			// JMP instruction
			case OPC_nJMP:
				REG_PC = opReg[0];
				flushvi();
				break;


			// BSBx instructions
			case OPC_nBSBB:
				writev(REG_SP - LN_LONG, REG_PC, LN_LONG, WACC);
				REG_SP -= LN_LONG;
				REG_PC += opReg[0];
				flushvi();
				break;
			case OPC_nBSBW:
				writev(REG_SP - LN_LONG, REG_PC, LN_LONG, WACC);
				REG_SP -= LN_LONG;
				REG_PC += opReg[0];
				flushvi();
				break;

			// JSB instruction
			case OPC_nJSB:
				writev(REG_SP - LN_LONG, REG_PC, LN_LONG, WACC);
				REG_SP -= LN_LONG;
				REG_PC = opReg[0];
				flushvi();
				break;

			// RSB instruction
			case OPC_nRSB:
				REG_PC  = readv(REG_SP, LN_LONG, RACC);
				REG_SP += LN_LONG;
				flushvi();
				break;

			// ACBx instructions
			case OPC_nACBB:
				src   = SXTB(opReg[0]);
				src1  = SXTB(opReg[1]);
				src2  = SXTB(opReg[2]);
				dst   = src2 + src1;
				storeb(opReg[3], dst);

				// Update condition codes
				SetNZ(ccReg, SXTB(dst), 0, (ccReg & CC_C));
				SetV_ADD(ccReg, SXTB(dst), SXTB(src2), SXTB(src1), SGN_BYTE);

				if ((src1 < 0) ? (dst >= src) : (dst <= src)) {
					REG_PC += opReg[4];
					flushvi();
				}
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND)) {
					dbg.log("%s: %02X + %02X => %02X: %s\n", deviceName(),
						ZXTB(src2), ZXTB(src1), ZXTB(dst), stringCC(ccReg));
					dbg.log("%s: %02X %s %02X: %s\n", deviceName(),
						ZXTB(dst), (src1 < 0) ? ">=" : "<=", ZXTB(src),
						((src1 < 0) ? (dst >= src) : (dst <= src)) ? "Jumped" : "Continue");
				}
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nACBW:
				src   = SXTW(opReg[0]);
				src1  = SXTW(opReg[1]);
				src2  = SXTW(opReg[2]);
				dst   = src2 + src1;
				storew(opReg[3], dst);

				// Update condition codes
				SetNZ(ccReg, SXTW(dst), 0, (ccReg & CC_C));
				SetV_ADD(ccReg, SXTW(dst), SXTW(src2), SXTW(src1), SGN_WORD);

				if ((src1 < 0) ? (dst >= src) : (dst <= src)) {
					REG_PC += opReg[4];
					flushvi();
				}
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND)) {
					dbg.log("%s: %04X + %04X => %04X: %s\n", deviceName(),
						ZXTW(src2), ZXTW(src1), ZXTW(dst), stringCC(ccReg));
					dbg.log("%s: %04X %s %04X: %s\n", deviceName(),
						ZXTW(dst), (src1 < 0) ? ">=" : "<=", ZXTW(src),
						((src1 < 0) ? (dst >= src) : (dst <= src)) ? "Jumped" : "Continue");
				}
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nACBL:
				src   = SXTL(opReg[0]);
				src1  = SXTL(opReg[1]);
				src2  = SXTL(opReg[2]);
				dst   = src2 + src1;
				storel(opReg[3], dst);

				// Update condition codes
				SetNZ(ccReg, SXTL(dst), 0, (ccReg & CC_C));
				SetV_ADD(ccReg, SXTL(dst), SXTL(src2), SXTL(src1), SGN_LONG);

				if ((src1 < 0) ? (dst >= src) : (dst <= src)) {
					REG_PC += opReg[4];
					flushvi();
				}
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND)) {
					dbg.log("%s: %08X + %08X => %08X: %s\n", deviceName(),
						ZXTL(src2), ZXTL(src1), ZXTL(dst), stringCC(ccReg));
					dbg.log("%s: %04X %s %04X: %s\n", deviceName(),
						ZXTL(dst), (src1 < 0) ? ">=" : "<=", ZXTL(src),
						((src1 < 0) ? (dst >= src) : (dst <= src)) ? "Jumped" : "Continue");
				}
#endif /* ENABLE_DEBUG */
				break;

			// CASEx - Case instructions
			case OPC_nCASEB:
				src1 = SXTB(opReg[0]);
				src2 = SXTB(opReg[1]);
				src  = SXTB(opReg[2]);
				dst  = src1 - src2;

				// Update Condition Codes
				SetNZ(ccReg, SXTB(dst), SXTB(src), 0);
				SetC(ccReg, ZXTB(dst), ZXTB(src));

				if (ZXTB(dst) <= ZXTB(src)) {
					brDisp = readv(REG_PC + (ZXTB(dst) << 1), LN_WORD, RACC);
					REG_PC += SXTW(brDisp);
				} else
					REG_PC += ((ZXTB(src) << 1) + 2);
				flushvi();
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: (%02X - %02X) => %02X <= %02X\n", deviceName(),
						ZXTB(src1), ZXTB(src2), ZXTB(dst), ZXTB(src));
#endif /* ENABLE_DEBUG */
//				if (ZXTB(dst) <= ZXTB(src))
//					printf("%s: %08X + %02X => %08X\n", deviceName(),
//						REG_PC, dst << 1, REG_PC + (dst << 1));
//				else
//					printf("%s: %08X + %02X + 2 => %08X\n", deviceName(),
//						REG_PC, src << 1, REG_PC + (src << 1) + 2);
				break;
			case OPC_nCASEW:
				src1 = SXTW(opReg[0]);
				src2 = SXTW(opReg[1]);
				src  = SXTW(opReg[2]);
				dst  = src1 - src2;

				// Update Condition Codes
				SetNZ(ccReg, SXTW(dst), SXTW(src), 0);
				SetC(ccReg, ZXTW(dst), ZXTW(src));

				if (ZXTW(dst) <= ZXTW(src)) {
					brDisp = readv(REG_PC + (ZXTW(dst) << 1), LN_WORD, RACC);
					REG_PC += SXTW(brDisp);
				} else
					REG_PC += ((ZXTW(src) << 1) + 2);
				flushvi();
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: (%04X - %04X) => %04X <= %04X\n", deviceName(),
						ZXTW(src1), ZXTW(src2), ZXTW(dst), ZXTW(src));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nCASEL:
				src1 = SXTL(opReg[0]);
				src2 = SXTL(opReg[1]);
				src  = SXTL(opReg[2]);
				dst  = src1 - src2;

				// Update Condition Codes
				SetNZ(ccReg, SXTL(dst), SXTL(src), 0);
				SetC(ccReg, ZXTL(dst), ZXTL(src));

				if (ZXTL(dst) <= ZXTL(src)) {
					brDisp = readv(REG_PC + (ZXTL(dst) << 1), LN_WORD, RACC);
					REG_PC += SXTW(brDisp);
				} else
					REG_PC += ((ZXTL(src) << 1) + 2);
				flushvi();
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: (%08X - %08X) => %08X <= %08X\n", deviceName(),
						ZXTL(src1), ZXTL(src2), ZXTL(dst), ZXTL(src));
#endif /* ENABLE_DEBUG */
				break;

			// AOBcc instructions
			case OPC_nAOBLEQ:
				src1 = SXTL(opReg[0]);
				src2 = SXTL(opReg[1]);
				dst  = src2 + 1;
				storel(opReg[2], dst);

				// Update Condition Codes
				SetNZ(ccReg, SXTL(dst), 0, (ccReg & CC_C));
				SetV_ADD(ccReg, SXTL(dst), SXTL(src2), 1, SGN_LONG);

				if (dst <= src1) {
					REG_PC += opReg[3];;
					flushvi();
				}
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND)) {
					dbg.log("%s: %08X + 1 => %08X <= %08X: %s\n", deviceName(),
						ZXTL(src2), ZXTL(dst), ZXTL(src1), stringCC(ccReg));
					if (dst <= src1)
						dbg.log("%s: Jump into PC %08X\n", deviceName(), REG_PC);
				}
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nAOBLSS:
				src1 = SXTL(opReg[0]);
				src2 = SXTL(opReg[1]);
				dst  = src2 + 1;
				storel(opReg[2], dst);

				// Update Condition Codes
				SetNZ(ccReg, SXTL(dst), 0, (ccReg & CC_C));
				SetV_ADD(ccReg, SXTL(dst), SXTL(src2), 1, SGN_LONG);

				if (dst < src1) {
					REG_PC += opReg[3];
					flushvi();
				}
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND)) {
					dbg.log("%s: %08X + 1 => %08X < %08X: %s\n", deviceName(),
						ZXTL(src2), ZXTL(dst), ZXTL(src1), stringCC(ccReg));
					if (dst < src1)
						dbg.log("%s: Jump into PC %08X\n", deviceName(), REG_PC);
				}
#endif /* ENABLE_DEBUG */
				break;

			// SOBcc instructions
			case OPC_nSOBGEQ:
				src = SXTL(opReg[0]);
				dst = src - 1;
				storel(opReg[1], dst);

				// Update Condition Codes
				SetNZ(ccReg, SXTL(dst), 0, (ccReg & CC_C));
				SetV_SUB(ccReg, SXTL(dst), SXTL(src), 1, SGN_LONG);

				if (dst >= 0) {
					REG_PC += opReg[2];
					flushvi();
				}
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND)) {
					dbg.log("%s: %08X - 1 => %08X >= 0: %s\n", deviceName(),
						ZXTL(src), ZXTL(dst), stringCC(ccReg));
					if (dst >= 0)
						dbg.log("%s: Jump into PC %08X\n", deviceName(), REG_PC);
				}
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nSOBGTR:
				src = SXTL(opReg[0]);
				dst = src - 1;
				storel(opReg[1], dst);

				// Update Condition Codes
				SetNZ(ccReg, SXTL(dst), 0, (ccReg & CC_C));
				SetV_SUB(ccReg, SXTL(dst), SXTL(src), 1, SGN_LONG);

				if (dst > 0) {
					REG_PC += opReg[2];
					flushvi();
				}
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND)) {
					dbg.log("%s: %08X - 1 => %08X > 0: %s\n", deviceName(),
						ZXTL(src), ZXTL(dst), stringCC(ccReg));
					if (dst > 0)
						dbg.log("%s: Jump into PC %08X\n", deviceName(), REG_PC);
				}
#endif /* ENABLE_DEBUG */
				break;

			// BBx instructions
			case OPC_nBBS:
				if (getBit() == 1) {
					REG_PC += opReg[2];
					flushvi();
				}
				break;
			case OPC_nBBC:
				if (getBit() == 0) {
					REG_PC += opReg[2];
					flushvi();
				}
				break;
			case OPC_nBBSSI:
			case OPC_nBBSS:
				if (setBit(1) == 1) {
					REG_PC += opReg[2];
					flushvi();
				}
				break;
			case OPC_nBBSC:
				if (setBit(0) == 1) {
					REG_PC += opReg[2];
					flushvi();
				}
				break;
			case OPC_nBBCS:
				if (setBit(1) == 0) {
					REG_PC += opReg[2];
					flushvi();
				}
				break;
			case OPC_nBBCCI:
			case OPC_nBBCC:
				if (setBit(0) == 0) {
					REG_PC += opReg[2];
					flushvi();
				}
				break;

			// BLBx instructions
			case OPC_nBLBS:
				src = opReg[0];
				if ((src & 1) == 1) {
					REG_PC += opReg[1];
					flushvi();
				}
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X & 1 => %d\n", deviceName(), src, src & 1);
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nBLBC:
				src = opReg[0];
				if ((src & 1) == 0) {
					REG_PC += opReg[1];
					flushvi();
				}
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X & 1 => %d\n", deviceName(), src, src & 1);
#endif /* ENABLE_DEBUG */
				break;

			// MOVx instructions
			// MOVAx instructions
			case OPC_nMOVB:
				dst = opReg[0];
				storeb(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTB(dst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move %02X: %s\n", deviceName(),
						ZXTB(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nMOVW:
				dst = opReg[0];
				storew(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTW(dst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move %04X: %s\n", deviceName(),
						ZXTW(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nMOVL:
			case OPC_nMOVAB:
			case OPC_nMOVAW:
			case OPC_nMOVAL:
			case OPC_nMOVAQ:
				dst = opReg[0];
				storel(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTL(dst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move %08X: %s\n", deviceName(),
						ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nMOVQ:
				dst1 = opReg[0];
				dst2 = opReg[1];
				storeq(opReg[2], dst1, dst2);

				// Update condition codes
				SetNZQ(ccReg, SXTL(dst1), SXTL(dst2), (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move %08X %08X: %s\n", deviceName(),
						ZXTL(dst1), ZXTL(dst2), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// MCOMx - Move complemented instructions
			case OPC_nMCOMB:
				src = SXTB(opReg[0]);
				dst = ~src;
				storeb(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTB(dst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move ~%02X => %02X: %s\n", deviceName(),
						ZXTB(src), ZXTB(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nMCOMW:
				src = SXTW(opReg[0]);
				dst = ~src;
				storew(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTW(dst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move ~%04X => %04X: %s\n", deviceName(),
						ZXTW(src), ZXTW(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nMCOML:
				src = SXTL(opReg[0]);
				dst = ~src;
				storel(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTL(dst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move ~%08X => %08X: %s\n", deviceName(),
						ZXTL(src), ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// MNEGx - Move negated instructions
			case OPC_nMNEGB:
				src = SXTB(opReg[0]);
				dst = -src;
				storeb(opReg[1], dst);

				// Update condition codes
				SetCC_SUB_B(ccReg, dst, 0, src);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move -%02X => %02X: %s\n", deviceName(),
						ZXTB(src), ZXTB(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nMNEGW:
				src = SXTW(opReg[0]);
				dst = -src;
				storew(opReg[1], dst);

				// Update condition codes
				SetCC_SUB_W(ccReg, dst, 0, src);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move -%04X => %04X: %s\n", deviceName(),
						ZXTW(src), ZXTW(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nMNEGL:
				src = SXTL(opReg[0]);
				dst = -src;
				storel(opReg[1], dst);

				// Update condition codes
				SetCC_SUB_L(ccReg, dst, 0, src);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move -%08X => %08X: %s\n", deviceName(),
						ZXTL(src), ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// MOVZx instructions
			case OPC_nMOVZBW:
				dst = ZXTB(opReg[0]);
				storew(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTW(dst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move %02X => %04X: %s\n", deviceName(),
						ZXTB(dst), ZXTW(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nMOVZBL:
				dst = ZXTB(opReg[0]);
				storel(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTL(dst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move %02X => %08X: %s\n", deviceName(),
						ZXTB(dst), ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nMOVZWL:
				dst = ZXTW(opReg[0]);
				storel(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTL(dst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move %04X => %08X: %s\n", deviceName(),
						ZXTW(dst), ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// CVTx - Convert instructions
			case OPC_nCVTBW:
				src = SXTB(opReg[0]);
				dst = SXTW(src);
				storew(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTW(dst), 0, 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %02X => %04X: %s\n", deviceName(),
						ZXTB(src), ZXTW(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nCVTBL:
				src = SXTB(opReg[0]);
				dst = SXTL(src);
				storel(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTW(dst), 0, 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %02X => %08X: %s\n", deviceName(),
						ZXTB(src), ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nCVTWB:
				src = SXTW(opReg[0]);
				dst = SXTB(src);
				storeb(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTB(dst), 0, 0);
				if (src < -128 || src > 127) {
					ccReg |= CC_V;
					if (psReg & PSW_IV)
						irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);
				}

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %04X => %02X: %s\n", deviceName(),
						ZXTW(src), ZXTB(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nCVTWL:
				src = SXTW(opReg[0]);
				dst = SXTL(src);
				storel(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTL(dst), 0, 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %04X => %08X: %s\n", deviceName(),
						ZXTW(src), ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nCVTLB:
				src = SXTL(opReg[0]);
				dst = SXTB(src);
				storeb(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTB(dst), 0, 0);
				if (src < -128 || src > 127) {
					ccReg |= CC_V;
					if (psReg & PSW_IV)
						irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);
				}

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X => %02X: %s\n", deviceName(),
						ZXTL(src), ZXTB(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nCVTLW:
				src = SXTL(opReg[0]);
				dst = SXTW(src);
				storew(opReg[1], dst);

				// Update condition codes
				SetNZ(ccReg, SXTW(dst), 0, 0);
				if (src < -128 || src > 127) {
					ccReg |= CC_V;
					if (psReg & PSW_IV)
						irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);
				}

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X => %04X: %s\n", deviceName(),
						ZXTL(src), ZXTW(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// CLRx instructions
			case OPC_nCLRB:
				storeb(opReg[0], 0);
				SetZ(ccReg);
				break;
			case OPC_nCLRW:
				storew(opReg[0], 0);
				SetZ(ccReg);
				break;
			case OPC_nCLRL:
				storel(opReg[0], 0);
				SetZ(ccReg);
				break;
			case OPC_nCLRQ:
				storeq(opReg[0], 0, 0);
				SetZ(ccReg);
				break;

			// CMPx instructions
			case OPC_nCMPB:
				src1 = SXTB(opReg[0]);
				src2 = SXTB(opReg[1]);

				// Update Condition Codes
				SetNZ(ccReg, SXTB(src1), SXTB(src2), 0);
				SetC(ccReg, ZXTB(src1), ZXTB(src2));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Compare %02X with %02X: %s\n", deviceName(),
						ZXTB(src1), ZXTB(src2), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nCMPW:
				src1 = SXTW(opReg[0]);
				src2 = SXTW(opReg[1]);

				// Update Condition Codes
				SetNZ(ccReg, SXTW(src1), SXTW(src2), 0);
				SetC(ccReg, ZXTW(src1), ZXTW(src2));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Compare %04X with %04X: %s\n", deviceName(),
						ZXTW(src1), ZXTW(src2), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nCMPL:
				src1 = SXTL(opReg[0]);
				src2 = SXTL(opReg[1]);

				// Update Condition Codes
				SetNZ(ccReg, SXTL(src1), SXTL(src2), 0);
				SetC(ccReg, ZXTL(src1), ZXTL(src2));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Compare %08X with %08X: %s\n", deviceName(),
						ZXTL(src1), ZXTL(src2), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

				// TSTx instructions
			case OPC_nTSTB:
				src = SXTB(opReg[0]);

				// Update condition codes
				SetNZ(ccReg, SXTB(src), 0, 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Test %02X: %s\n", deviceName(),
						ZXTB(src), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nTSTW:
				src = SXTW(opReg[0]);

				// Update condition codes
				SetNZ(ccReg, SXTW(src), 0, 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Test %04X: %s\n", deviceName(),
						ZXTW(src), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nTSTL:
				src = SXTL(opReg[0]);

				// Update condition codes
				SetNZ(ccReg, SXTL(src), 0, 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Test %08X: %s\n", deviceName(),
						ZXTL(src), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// INCx instructions
			case OPC_nINCB:
				src = SXTB(opReg[0]);
				dst = src + 1;
				storeb(opReg[1], dst);

				// Update condition codes
				SetCC_ADD_B(ccReg, dst, src, 1);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %02X + 1 => %02X: %s\n",
						deviceName(), ZXTB(src), ZXTB(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nINCW:
				src = SXTW(opReg[0]);
				dst = src + 1;
				storew(opReg[1], dst);

				// Update condition codes
				SetCC_ADD_W(ccReg, dst, src, 1);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %04X + 1 => %04X: %s\n",
						deviceName(), ZXTW(src), ZXTW(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nINCL:
				src = SXTL(opReg[0]);
				dst = src + 1;
				storel(opReg[1], dst);

				// Update condition codes
				SetCC_ADD_L(ccReg, dst, src, 1);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X + 1 => %08X: %s\n",
						deviceName(), ZXTL(src), ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// DECx instructions
			case OPC_nDECB:
				src = SXTB(opReg[0]);
				dst = src - 1;
				storeb(opReg[1], dst);

				// Update condition codes
				SetCC_SUB_B(ccReg, dst, src, 1);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X - 1 => %08X: %s\n",
						deviceName(), ZXTB(src), ZXTB(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nDECW:
				src = SXTW(opReg[0]);
				dst = src - 1;
				storew(opReg[1], dst);

				// Update condition codes
				SetCC_SUB_W(ccReg, dst, src, 1);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %04X - 1 => %04X: %s\n",
						deviceName(), ZXTW(src), ZXTW(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nDECL:
				src = SXTL(opReg[0]);
				dst = src - 1;
				storel(opReg[1], dst);

				// Update condition codes
				SetCC_SUB_L(ccReg, dst, src, 1);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X - 1 => %08X: %s\n",
						deviceName(), ZXTL(src), ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// ADAWI instruction
			case OPC_nADAWI:
				src = SXTW(opReg[0]);
				if (OP_ISREG(opReg[1])) {
					tmp = SXTW(gpReg[~opReg[1]].l);
					dst = src + tmp;
					gpReg[~opReg[1]].l = ZXTW(dst);
				} else {
					if (opReg[1] & 1)
						throw RSVD_OPND_FAULT;
					tmp = SXTW(readv(opReg[1], LN_WORD, RACC));
					dst = src + tmp;
					writev(opReg[1], dst, LN_WORD, WACC);
				}

				// Update condition codes
				SetCC_ADD_W(ccReg, dst, src, tmp);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %04X + %04X => %04X: %s\n", deviceName(),
						ZXTW(src), ZXTW(tmp), ZXTW(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// ADDx instructions
			case OPC_nADDB2:
			case OPC_nADDB3:
				src1 = SXTB(opReg[0]);
				src2 = SXTB(opReg[1]);
				dst  = src2 + src1;
				storeb(opReg[2], dst);

				// Update condition codes
				SetCC_ADD_B(ccReg, dst, src2, src1);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %02X + %02X => %02X: %s\n", deviceName(),
						ZXTB(src1), ZXTB(src2), ZXTB(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nADDW2:
			case OPC_nADDW3:
				src1 = SXTW(opReg[0]);
				src2 = SXTW(opReg[1]);
				dst  = src2 + src1;
				storew(opReg[2], dst);

				// Update condition codes
				SetCC_ADD_W(ccReg, dst, src2, src1);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %04X + %04X => %04X: %s\n", deviceName(),
						ZXTW(src1), ZXTW(src2), ZXTW(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nADDL2:
			case OPC_nADDL3:
				src1 = SXTL(opReg[0]);
				src2 = SXTL(opReg[1]);
				dst  = src2 + src1;
				storel(opReg[2], dst);

				// Update condition codes
				SetCC_ADD_L(ccReg, dst, src2, src1);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X + %08X => %08X: %s\n", deviceName(),
						ZXTL(src1), ZXTL(src2), ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// SUBx instructions
			case OPC_nSUBB2:
			case OPC_nSUBB3:
				src1 = SXTB(opReg[0]);
				src2 = SXTB(opReg[1]);
				dst  = src2 - src1;
				storeb(opReg[2], dst);

				// Update condition codes
				SetCC_SUB_B(ccReg, dst, src2, src1);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %02X - %02X => %02X: %s\n", deviceName(),
						ZXTB(src1), ZXTB(src2), ZXTB(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nSUBW2:
			case OPC_nSUBW3:
				src1 = SXTW(opReg[0]);
				src2 = SXTW(opReg[1]);
				dst  = src2 - src1;
				storew(opReg[2], dst);

				// Update condition codes
				SetCC_SUB_W(ccReg, dst, src2, src1);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %04X - %04X => %04X: %s\n", deviceName(),
						ZXTW(src1), ZXTW(src2), ZXTW(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nSUBL2:
			case OPC_nSUBL3:
				src1 = SXTL(opReg[0]);
				src2 = SXTL(opReg[1]);
				dst  = src2 - src1;
				storel(opReg[2], dst);

				// Update condition codes
				SetCC_SUB_L(ccReg, dst, src2, src1);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X - %08X => %08X: %s\n", deviceName(),
						ZXTL(src1), ZXTL(src2), ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// MULx - Multiply instructions
			case OPC_nMULB2:
			case OPC_nMULB3:
				src1 = SXTB(opReg[0]);
				src2 = SXTB(opReg[1]);
				dst  = src2 * src1;
				storeb(opReg[2], dst);

				// Update condition codes
				SetNZ(ccReg, SXTB(dst), 0, 0);
				if (dst < SCHAR_MIN || dst > SCHAR_MAX) {
					ccReg |= CC_V;
					if (psReg & PSW_IV)
						irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);
				}

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %02X * %02X => %02X: %s\n", deviceName(),
						ZXTB(src2), ZXTB(src1), ZXTB(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nMULW2:
			case OPC_nMULW3:
				src1 = SXTW(opReg[0]);
				src2 = SXTW(opReg[1]);
				dst  = src2 * src1;
				storew(opReg[2], dst);

				// Update condition codes
				SetNZ(ccReg, SXTW(dst), 0, 0);
				if (dst < SHRT_MIN || dst > SHRT_MAX) {
					ccReg |= CC_V;
					if (psReg & PSW_IV)
						irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);
				}

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %02X * %02X => %02X: %s\n", deviceName(),
						ZXTW(src2), ZXTW(src1), ZXTW(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nMULL2:
			case OPC_nMULL3:
				srcq1 = SXTL(opReg[0]);
				srcq2 = SXTL(opReg[1]);
				dstq  = srcq2 * srcq1;
				storel(opReg[2], SXTL(dstq));

				// Update condition codes
				SetNZ(ccReg, SXTL(dstq), 0, 0);
//				if (SXTL(dstq >> 32) != (SXTL(dstq) & SGN_LONG) ? -1LL : 0LL)
//					ccReg |= CC_V;
				if (dstq < LONG_MIN || dstq > LONG_MAX) {
					ccReg |= CC_V;
					if (psReg & PSW_IV)
						irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);
				}
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X * %08X => %08X: %s\n", deviceName(),
						ZXTL(srcq2), ZXTL(srcq1), ZXTL(dstq), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// DIVx - Divide instructions
			case OPC_nDIVB2:
			case OPC_nDIVB3:
				src1 = SXTB(opReg[0]);
				src2 = SXTB(opReg[1]);
				if ((src1 == 0) || ((src1 == -1) && (src2 == SCHAR_MIN))) {
					dst   = src2;
					ovflg = true;
					if (src1 == 0)
						irqFlags |= IRQ_SETTRAP(TRAP_INTDIV);
					else if (psReg & PSW_IV)
						irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);
				} else {
					dst   = src2 / src1;
					ovflg = false;
				}
				storeb(opReg[2], dst);

				// Update condition codes
				SetNZ(ccReg, SXTB(dst), 0, (ovflg ? CC_V : 0));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %02X / %02X => %02X: %s\n", deviceName(),
						ZXTB(src2), ZXTB(src1), ZXTB(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nDIVW2:
			case OPC_nDIVW3:
				src1 = SXTW(opReg[0]);
				src2 = SXTW(opReg[1]);
				if ((src1 == 0) || ((src1 == -1) && (src2 == SHRT_MIN))) {
					dst   = src2;
					ovflg = true;
					if (src1 == 0)
						irqFlags |= IRQ_SETTRAP(TRAP_INTDIV);
					else if (psReg & PSW_IV)
						irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);
				} else {
					dst   = src2 / src1;
					ovflg = false;
				}
				storew(opReg[2], dst);

				// Update condition codes
				SetNZ(ccReg, SXTW(dst), 0, (ovflg ? CC_V : 0));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %04X / %04X => %04X: %s\n", deviceName(),
						ZXTW(src2), ZXTW(src1), ZXTW(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nDIVL2:
			case OPC_nDIVL3:
				src1 = SXTL(opReg[0]);
				src2 = SXTL(opReg[1]);
				if ((src1 == 0) || ((src1 == -1) && (src2 == LONG_MIN))) {
					dst   = src2;
					ovflg = true;
					if (src1 == 0)
						irqFlags |= IRQ_SETTRAP(TRAP_INTDIV);
					else if (psReg & PSW_IV)
						irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);
				} else {
					dst   = src2 / src1;
					ovflg = false;
				}
				storel(opReg[2], dst);

				// Update condition codes
				SetNZ(ccReg, SXTL(dst), 0, (ovflg ? CC_V : 0));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X / %08X => %08X: %s\n", deviceName(),
						ZXTL(src2), ZXTL(src1), ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// EMUL - Extended multiply instruction
			case OPC_nEMUL:
				srcq1 = SXTL(opReg[0]);
				srcq2 = SXTL(opReg[1]);
				srcq  = SXTL(opReg[2]);
				dstq  = (srcq2 * srcq1) + srcq;
				storeq(opReg[3], ZXTL(dstq), ZXTL(dstq >> 32));

				// Update condition codes
				SetNZ(ccReg, SXTQ(dstq), 0LL, 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: (%08X * %08X) + %08X => %08X %08X: %s\n", deviceName(),
						ZXTL(srcq2), ZXTL(srcq1), ZXTL(srcq), ZXTL(dstq >> 32), ZXTL(dstq),
						stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			// EDIV - Extended divide instruction
			case OPC_nEDIV:
				srcq1 = SXTL(opReg[0]);
				srcq2 = (ZXTQ(opReg[2]) << 32) | ZXTQ(opReg[1]);
				ovflg = false;
				if ((srcq1 == 0) || ((srcq1 == -1LL) && (srcq2 == LLONG_MIN))) {
					ovflg = true;
				} else {
//					printf("%s: %08X >= %08X\n", deviceName(),
//						abs(srcq2 >> 32), abs(srcq1));
					if (abs(srcq2 >> 32) >= abs(srcq1) && ZXTL(srcq1) != SGN_LONG)
						ovflg = true;
					else {
						dstq1 = srcq2 / srcq1;
						dstq2 = srcq2 % srcq1;
						if ((dstq1 < LONG_MIN) || (dstq1 > LONG_MAX))
							ovflg = true;
					}
				}

				if (ovflg == true) {
					dstq1 = srcq2;
					dstq2 = 0;
					if (srcq1 == 0)
						irqFlags |= IRQ_SETTRAP(TRAP_INTDIV);
					else if (psReg & PSW_IV)
						irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);
				}

				storel(opReg[3], dstq1);
				storel(opReg[4], dstq2);

				// Update condition codes
				SetNZ(ccReg, SXTL(dstq1), 0, (ovflg ? CC_V : 0));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X %08X / %08X => %08X R %08X: %s\n", deviceName(),
						ZXTL(srcq2 >> 32), ZXTL(srcq2), ZXTL(srcq1), ZXTL(dstq1), ZXTL(dstq2),
						stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// ADWC/SBWC instructions
			case OPC_nADWC:
				src1  = SXTL(opReg[0]);
				src2  = SXTL(opReg[1]);
				carry = ccReg & CC_C;
				dst   = src2 + src1 + carry;
				storel(opReg[2], dst);

				// Update condition codes
				SetCC_ADD_L(ccReg, dst, src2, src1);
				if ((dst == src2) && src1 != 0)
					ccReg |= CC_C;

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X + %08X + %d => %08X: %s\n", deviceName(),
						ZXTL(src1), ZXTL(src2), carry, ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nSBWC:
				src1  = SXTL(opReg[0]);
				src2  = SXTL(opReg[1]);
				carry = ccReg & CC_C;
				dst   = src2 - src1 - carry;
				storel(opReg[2], dst);

				// Update condition codes
				SetCC_SUB_L(ccReg, dst, src2, src1);
				if ((src1 == src2) && dst != 0)
					ccReg |= CC_C;

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X - %08X - %d => %08X: %s\n", deviceName(),
						ZXTL(src1), ZXTL(src2), carry, ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// BICx instructions
			case OPC_nBICB2:
			case OPC_nBICB3:
				mask = ZXTB(opReg[0]);
				usrc = ZXTB(opReg[1]);
				udst = usrc & ~mask;
				storeb(opReg[2], udst);

				// Update condition codes
				SetNZ(ccReg, SXTB(udst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %02X & ~%02X => %02X: %s\n", deviceName(),
						ZXTB(usrc), ZXTB(mask), ZXTB(udst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nBICW2:
			case OPC_nBICW3:
				mask = ZXTW(opReg[0]);
				usrc = ZXTW(opReg[1]);
				udst = usrc & ~mask;
				storew(opReg[2], udst);

				// Update condition codes
				SetNZ(ccReg, SXTW(udst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %04X & ~%04X => %04X: %s\n", deviceName(),
						ZXTW(usrc), ZXTW(mask), ZXTW(udst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nBICL2:
			case OPC_nBICL3:
				mask = ZXTL(opReg[0]);
				usrc = ZXTL(opReg[1]);
				udst = usrc & ~mask;
				storel(opReg[2], udst);

				// Update condition codes
				SetNZ(ccReg, SXTL(udst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X & ~%08X => %08X: %s\n", deviceName(),
						ZXTL(usrc), ZXTL(mask), ZXTL(udst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// BISx instructions
			case OPC_nBISB2:
			case OPC_nBISB3:
				mask = ZXTB(opReg[0]);
				usrc = ZXTB(opReg[1]);
				udst = usrc | mask;
				storeb(opReg[2], udst);

				// Update condition codes
				SetNZ(ccReg, SXTB(udst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %02X | %02X => %02X: %s\n", deviceName(),
						ZXTB(usrc), ZXTB(mask), ZXTB(udst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nBISW2:
			case OPC_nBISW3:
				mask = ZXTW(opReg[0]);
				usrc = ZXTW(opReg[1]);
				udst = usrc | mask;
				storew(opReg[2], udst);

				// Update condition codes
				SetNZ(ccReg, SXTW(udst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %04X | %04X => %04X: %s\n", deviceName(),
						ZXTW(usrc), ZXTW(mask), ZXTW(udst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nBISL2:
			case OPC_nBISL3:
				mask = ZXTL(opReg[0]);
				usrc = ZXTL(opReg[1]);
				udst = usrc | mask;
				storel(opReg[2], udst);

				// Update condition codes
				SetNZ(ccReg, SXTL(udst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X | %08X => %08X: %s\n", deviceName(),
						ZXTL(usrc), ZXTL(mask), ZXTL(udst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// BITx instructions
			case OPC_nBITB:
				mask = ZXTB(opReg[0]);
				usrc = ZXTB(opReg[1]);
				udst = usrc & mask;

				// Update condition codes
				SetNZ(ccReg, SXTB(udst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %02X & %02X => %02X: %s\n", deviceName(),
						ZXTB(usrc), ZXTB(mask), ZXTB(udst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nBITW:
				mask = ZXTW(opReg[0]);
				usrc = ZXTW(opReg[1]);
				udst = usrc & mask;

				// Update condition codes
				SetNZ(ccReg, SXTW(udst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %04X & %04X => %04X: %s\n", deviceName(),
						ZXTW(usrc), ZXTW(mask), ZXTW(udst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nBITL:
				mask = ZXTL(opReg[0]);
				usrc = ZXTL(opReg[1]);
				udst = usrc & mask;

				// Update condition codes
				SetNZ(ccReg, SXTL(udst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X & %08X => %08X: %s\n", deviceName(),
						ZXTL(usrc), ZXTL(mask), ZXTL(udst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// XORx instructions
			case OPC_nXORB2:
			case OPC_nXORB3:
				mask = ZXTB(opReg[0]);
				usrc = ZXTB(opReg[1]);
				udst = usrc ^ mask;
				storeb(opReg[2], udst);

				// Update condition codes
				SetNZ(ccReg, SXTB(udst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %02X ^ %02X => %02X: %s\n", deviceName(),
						ZXTB(usrc), ZXTB(mask), ZXTB(udst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nXORW2:
			case OPC_nXORW3:
				mask = ZXTW(opReg[0]);
				usrc = ZXTW(opReg[1]);
				udst = usrc ^ mask;
				storew(opReg[2], udst);

				// Update condition codes
				SetNZ(ccReg, SXTW(udst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %04X ^ %04X => %04X: %s\n", deviceName(),
						ZXTW(usrc), ZXTW(mask), ZXTW(udst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nXORL2:
			case OPC_nXORL3:
				mask = ZXTL(opReg[0]);
				usrc = ZXTL(opReg[1]);
				udst = usrc ^ mask;
				storel(opReg[2], udst);

				// Update condition codes
				SetNZ(ccReg, SXTL(udst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X ^ %08X => %08X: %s\n", deviceName(),
						ZXTL(usrc), ZXTL(mask), ZXTL(udst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// ASHx/ROTL - Shift instructions
			case OPC_nASHL:
				cnt   = SXTB(opReg[0]);
				src   = SXTL(opReg[1]);
				ovflg = false;
				if (cnt == 0) {
					dst = src;
				} else if (cnt < 0) {
					dst = (cnt > -32) ? (src >> -cnt) : (src < 0) ? -1 : 0;
				} else {
					if (cnt < 32) {
						dst = ZXTL(src) << cnt;
						ovflg = (src != (dst >> cnt));
					} else {
						dst = 0;
						ovflg = (src != 0);
					}
				}
				storel(opReg[2], dst);

				// Update condition codes
				SetNZ(ccReg, SXTL(dst), 0, 0);
				if (ovflg) {
					ccReg |= CC_V;
					if (psReg & PSW_IV)
						irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);
				}

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X %s %d => %08X: %s\n", deviceName(),
						ZXTL(src), ((cnt < 0) ? ">>" : "<<"), abs(cnt),
						ZXTL(dst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nASHQ:
				cnt    = SXTB(opReg[0]);
				srcq  = (ZXTQ(opReg[2]) << 32) | ZXTL(opReg[1]);
				ovflg = false;
				if (cnt == 0) {
					dstq = srcq;
				} else if (cnt < 0) {
					dstq = (cnt > -64) ? (srcq >> -cnt) : (srcq < 0) ? -1LL : 0LL;
				} else {
					if (cnt < 64) {
						dstq  = srcq << cnt;
						ovflg = (srcq != (dstq >> cnt));
					} else {
						dstq  = 0LL;
						ovflg = (srcq != 0);
					}
				}
				storeq(opReg[3], SXTL(dstq), SXTL(dstq >> 32));

				// Update condition codes
				SetNZ(ccReg, SXTQ(dstq), 0LL, 0);
				if (ovflg) {
					ccReg |= CC_V;
					if (psReg & PSW_IV)
						irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);
				}

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X %08X %s %d => %08X %08X: %s\n", deviceName(),
						ZXTL(srcq >> 32), ZXTL(srcq), ((cnt < 0) ? ">>" : "<<"), abs(cnt),
						ZXTL(dstq >> 32), ZXTL(dstq), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nROTL:
				cnt  = ZXTB(opReg[0]) % 32;
				usrc = ZXTL(opReg[1]);
				udst = (cnt != 0) ? ((usrc << cnt) | (usrc >> (32 - cnt))) : usrc;
				storel(opReg[2], udst);

				// Update condition codes
				SetNZ(ccReg, SXTL(udst), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X %s %d => %08X: %s\n", deviceName(),
						ZXTL(usrc), ((cnt < 0) ? ">>" : "<<"), abs(cnt),
						ZXTL(udst), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// CMPV - Compare field instructions
			case OPC_nCMPV:
				dst = getField(true);
				src = SXTL(opReg[3]);

				// Update condition codes
				SetNZ(ccReg, SXTL(dst), SXTL(src), 0);
				SetC(ccReg, ZXTL(dst), ZXTL(src));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Compare %08X with %08X: %s\n", deviceName(),
						ZXTL(dst), ZXTL(src), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nCMPZV:
				dst = getField(false);
				src = SXTL(opReg[3]);

				// Update condition codes
				SetNZ(ccReg, SXTL(dst), SXTL(src), 0);
				SetC(ccReg, ZXTL(dst), ZXTL(src));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Compare %08X with %08X: %s\n", deviceName(),
						ZXTL(dst), ZXTL(src), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// EXTV - Extract field instructions
			case OPC_nEXTV:
				src = getField(true);
				storel(opReg[3], src);

				// Update condition codes
				SetNZ(ccReg, SXTL(src), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Extract %08X: %s\n", deviceName(),
						ZXTL(src), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nEXTZV:
				src = getField(false);
				storel(opReg[3], src);

				// Update condition codes
				SetNZ(ccReg, SXTL(src), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Extract %08X: %s\n", deviceName(),
						ZXTL(src), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// FFS/FFC - First find instruction
			case OPC_nFFS:
				usrc1 = ZXTL(opReg[0]);
				usrc2 = ZXTB(opReg[1]);
				if (usrc2 > 0) {
					int idx;
					usrc = getField(false);
					for (idx = 0; idx < usrc2; idx++)
						if ((usrc >> idx) & 1)
							break;
					udst = usrc1 + idx;
				} else {
					usrc = 0;
					udst = usrc1;
				}
				storel(opReg[3], udst);
				ccReg = usrc != 0 ? 0 : CC_Z;
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Find %08X<%d:%d> => %d: %s\n", deviceName(),
						ZXTL(usrc), ZXTL(usrc1), ZXTB(usrc2), udst,
						stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;
			case OPC_nFFC:
				usrc1 = ZXTL(opReg[0]);
				usrc2 = ZXTB(opReg[1]);
				if (usrc2 > 0) {
					int idx;
					usrc = getField(false) ^ mskList[usrc2];
//					printf("%s: %08X\n", deviceName(), usrc);
					for (idx = 0; idx < usrc2; idx++)
						if ((usrc >> idx) & 1)
							break;
					udst = usrc1 + idx;
				} else {
					usrc = 0;
					udst = usrc1;
				}
				storel(opReg[3], udst);
				ccReg = usrc != 0 ? 0 : CC_Z;
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Find %08X<%d:%d> => %d: %s\n", deviceName(),
						ZXTL(usrc), ZXTL(usrc1), ZXTB(usrc2), udst,
						stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// INSV - Insert field instruction
			case OPC_nINSV:
				putField();
				break;

			// ******************
			// Stack instructions
			// ******************

			// PUSHx/PUSHAx - Push instruction
			case OPC_nPUSHL:
			case OPC_nPUSHAB:
			case OPC_nPUSHAW:
			case OPC_nPUSHAL:
			case OPC_nPUSHAQ:
				src = opReg[0];
				writev(REG_SP - LN_LONG, src, LN_LONG, WACC);
				REG_SP -= LN_LONG;

				// Update condition codes
				SetNZ(ccReg, SXTL(src), 0, (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Push %08X to SP (%08X): %s\n", deviceName(),
						src, REG_SP + LN_LONG, stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			// PUSHR/POPR - Push/pop register instructions
			case OPC_nPUSHR:
				mask = SXTL(opReg[0]) & STK_MASK;
				cnt  = 0;

				// Check access for page faults
				for (int idx = REG_nSP; idx >= REG_nR0; idx--)
					if ((mask >> idx) & 1)
						cnt += LN_LONG;
				readv(REG_SP - cnt, LN_BYTE, WACC);

				// Push registers into stack
				for (int idx = REG_nSP; idx >= REG_nR0; idx--) {
					if ((mask >> idx) & 1) {
						writev(REG_SP - LN_LONG, gpReg[idx].l, LN_LONG, WACC);
						REG_SP -= LN_LONG;
#ifdef ENABLE_DEBUG
						if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
							dbg.log("%s: R%d %08X => (SP) %08X\n", deviceName(),
								idx, ZXTL(gpReg[idx].l), ZXTL(REG_SP));
#endif /* ENABLE_DEBUG */
					}
				}
				break;
			case OPC_nPOPR:
				mask = SXTL(opReg[0]) & STK_MASK;
				cnt  = 0;

				// Check access for page faults
				for (int idx = REG_nR0; idx <= REG_nSP; idx++)
					if ((mask >> idx) & 1)
						cnt += LN_LONG;
				readv(REG_SP + cnt - 1, LN_BYTE, RACC);

				// Push registers into stack
				for (int idx = REG_nR0; idx <= REG_nSP; idx++) {
					if ((mask >> idx) & 1) {
						gpReg[idx].l = readv(REG_SP, LN_LONG, RACC);
#ifdef ENABLE_DEBUG
						if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
							dbg.log("%s: R%d %08X <= (SP) %08X\n", deviceName(),
								idx, ZXTL(gpReg[idx].l), ZXTL(REG_SP));
#endif /* ENABLE_DEBUG */
						if (idx < REG_nSP)
							REG_SP += LN_LONG;
					}
				}
				break;

			// CALLx - Call instructions
			case OPC_nCALLS:
				call(true);
				break;
			case OPC_nCALLG:
				call(false);
				break;

			// RET - Return instruction
			case OPC_nRET:
				ret();
				break;

			// *********************
			// Floating Instructions
			// *********************

			// Convert instructions

			case OPC_nCVTBF:
				usrcx[0] = SXTB(opReg[0]);
				if ((sts = vaxfp_t::converti(usrcx[0], udstx, SFP_TYPE)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %02X: %s\n", deviceName(),
								ZXTB(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storel(opReg[1], udstx[0]);

				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %02X to %08X: %s\n", deviceName(),
						ZXTB(usrcx[0]), ZXTL(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTWF:
				usrcx[0] = SXTW(opReg[0]);
				if ((sts = vaxfp_t::converti(usrcx[0], udstx, SFP_TYPE)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %04X: %s\n", deviceName(),
								ZXTW(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storel(opReg[1], udstx[0]);

				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %04X to %08X: %s\n", deviceName(),
						ZXTW(usrcx[0]), ZXTL(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTLF:
				usrcx[0] = SXTL(opReg[0]);
				if ((sts = vaxfp_t::converti(usrcx[0], udstx, SFP_TYPE)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storel(opReg[1], udstx[0]);

				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X to %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTFB:
				usrcx[0] = opReg[0];
				if ((sts = vaxfp_t::convertfi(usrcx, SFP_TYPE, udstx, VFP_BYTE, &flg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storeb(opReg[1], udstx[0]);

				// Update condition codes
				SetNZ(ccReg, SXTB(udstx[0]), 0, flg);
				if ((ccReg & CC_V) && (psReg & PSW_IV))
					irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X to %02X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTB(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTFW:
				usrcx[0] = opReg[0];
				if ((sts = vaxfp_t::convertfi(usrcx, SFP_TYPE, udstx, VFP_WORD, &flg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storew(opReg[1], udstx[0]);

				// Update condition codes
				SetNZ(ccReg, SXTW(udstx[0]), 0, flg);
				if ((ccReg & CC_V) && (psReg & PSW_IV))
					irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X to %04X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTW(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTFL:
				usrcx[0] = opReg[0];
				if ((sts = vaxfp_t::convertfi(usrcx, SFP_TYPE, udstx, VFP_LONG, &flg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storel(opReg[1], udstx[0]);

				// Update condition codes
				SetNZ(ccReg, SXTL(udstx[0]), 0, flg);
				if ((ccReg & CC_V) && (psReg & PSW_IV))
					irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X to %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTRFL:
				usrcx[0] = opReg[0];
				if ((sts = vaxfp_t::convertfi(usrcx, SFP_TYPE, udstx, VFP_RLONG, &flg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storel(opReg[1], udstx[0]);

				// Update condition codes
				SetNZ(ccReg, SXTL(udstx[0]), 0, flg);
				if ((ccReg & CC_V) && (psReg & PSW_IV))
					irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X to %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTBD:
				usrcx[0] = SXTB(opReg[0]);
				if ((sts = vaxfp_t::converti(usrcx[0], udstx, DFP_TYPE)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %02X: %s\n", deviceName(),
								ZXTB(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storeqp(opReg[1], udstx);

				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %02X to %08X %08X: %s\n", deviceName(),
						ZXTB(usrcx[0]), ZXTL(udstx[0]), ZXTL(udstx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTWD:
				usrcx[0] = SXTW(opReg[0]);
				if ((sts = vaxfp_t::converti(usrcx[0], udstx, DFP_TYPE)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %04X: %s\n", deviceName(),
								ZXTW(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storeqp(opReg[1], udstx);

				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %04X to %08X %08X: %s\n", deviceName(),
						ZXTW(usrcx[0]), ZXTL(udstx[0]), ZXTL(udstx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTLD:
				usrcx[0] = SXTL(opReg[0]);
				if ((sts = vaxfp_t::converti(usrcx[0], udstx, DFP_TYPE)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storeqp(opReg[1], udstx);

				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X to %08X %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(udstx[0]), ZXTL(udstx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTDB:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];

				if ((sts = vaxfp_t::convertfi(usrcx, DFP_TYPE, udstx, VFP_BYTE, &flg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ZXTL(usrcx[1]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storeb(opReg[2], udstx[0]);

				// Update condition codes
				SetNZ(ccReg, SXTB(udstx[0]), 0, flg);
				if ((ccReg & CC_V) && (psReg & PSW_IV))
					irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X %08X to %02X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTB(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTDW:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];

				if ((sts = vaxfp_t::convertfi(usrcx, DFP_TYPE, udstx, VFP_WORD, &flg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ZXTL(usrcx[1]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storew(opReg[2], udstx[0]);

				// Update condition codes
				SetNZ(ccReg, SXTW(udstx[0]), 0, flg);
				if ((ccReg & CC_V) && (psReg & PSW_IV))
					irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X %08X to %04X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTW(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTDL:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];

				if ((sts = vaxfp_t::convertfi(usrcx, DFP_TYPE, udstx, VFP_LONG, &flg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ZXTL(usrcx[1]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storel(opReg[2], udstx[0]);

				// Update condition codes
				SetNZ(ccReg, SXTL(udstx[0]), 0, flg);
				if ((ccReg & CC_V) && (psReg & PSW_IV))
					irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X %08X to %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTRDL:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];

				if ((sts = vaxfp_t::convertfi(usrcx, DFP_TYPE, udstx, VFP_RLONG, &flg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ZXTL(usrcx[1]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storel(opReg[2], udstx[0]);

				// Update condition codes
				SetNZ(ccReg, SXTL(udstx[0]), 0, flg);
				if ((ccReg & CC_V) && (psReg & PSW_IV))
					irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X %08X to %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;


			case OPC_nCVTBG:
				usrcx[0] = SXTB(opReg[0]);

				if ((sts = vaxfp_t::converti(usrcx[0], udstx, GFP_TYPE)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %02X: %s\n", deviceName(),
								ZXTB(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storeqp(opReg[1], udstx);

				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %02X to %08X %08X: %s\n", deviceName(),
						ZXTB(usrcx[0]), ZXTL(udstx[0]), ZXTL(udstx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTWG:
				usrcx[0] = SXTW(opReg[0]);
				if ((sts = vaxfp_t::converti(usrcx[0], udstx, GFP_TYPE)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %04X: %s\n", deviceName(),
								ZXTW(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storeqp(opReg[1], udstx);

				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %04X to %08X %08X: %s\n", deviceName(),
						ZXTW(usrcx[0]), ZXTL(udstx[0]), ZXTL(udstx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTLG:
				usrcx[0] = SXTL(opReg[0]);
				if ((sts = vaxfp_t::converti(usrcx[0], udstx, GFP_TYPE)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storeqp(opReg[1], udstx);

				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X to %08X %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(udstx[0]), ZXTL(udstx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;


			case OPC_nCVTGB:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];

				if ((sts = vaxfp_t::convertfi(usrcx, GFP_TYPE, udstx, VFP_BYTE, &flg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ZXTL(usrcx[1]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storeb(opReg[2], udstx[0]);

				// Update condition codes
				SetNZ(ccReg, SXTB(udstx[0]), 0, flg);
				if ((ccReg & CC_V) && (psReg & PSW_IV))
					irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X %08X to %02X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTB(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTGW:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];

				if ((sts = vaxfp_t::convertfi(usrcx, GFP_TYPE, udstx, VFP_WORD, &flg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ZXTL(usrcx[1]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storew(opReg[2], udstx[0]);

				// Update condition codes
				SetNZ(ccReg, SXTW(udstx[0]), 0, flg);
				if ((ccReg & CC_V) && (psReg & PSW_IV))
					irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X %08X to %04X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTW(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTGL:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];

				if ((sts = vaxfp_t::convertfi(usrcx, GFP_TYPE, udstx, VFP_LONG, &flg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ZXTL(usrcx[1]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storel(opReg[2], udstx[0]);

				// Update condition codes
				SetNZ(ccReg, SXTL(udstx[0]), 0, flg);
				if ((ccReg & CC_V) && (psReg & PSW_IV))
					irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X %08X to %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTRGL:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];

				if ((sts = vaxfp_t::convertfi(usrcx, GFP_TYPE, udstx, VFP_RLONG, &flg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ZXTL(usrcx[1]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storel(opReg[2], udstx[0]);

				// Update condition codes
				SetNZ(ccReg, SXTL(udstx[0]), 0, flg);
				if ((ccReg & CC_V) && (psReg & PSW_IV))
					irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X %08X to %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;


			case OPC_nCVTBH:
			case OPC_nCVTWH:
			case OPC_nCVTLH:
			case OPC_nCVTHB:
			case OPC_nCVTHW:
			case OPC_nCVTHL:
			case OPC_nCVTRHL:
				throw RSVD_INST_FAULT;

			// Note: No CVTDG and CVTGD instructions for VAX architecture
			case OPC_nCVTFD:
				usrcx[0] = opReg[0];

				if ((sts = vaxfp_t::convertfd(usrcx, udstx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storeqp(opReg[1], udstx);
				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X to %08X %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(udstx[0]), ZXTL(udstx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTDF:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];

				if ((sts = vaxfp_t::convertdf(usrcx, udstx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ZXTL(usrcx[1]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storel(opReg[2], udstx[0]);
				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X %08X to %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTFG:
				usrcx[0] = opReg[0];

				if ((sts = vaxfp_t::convertfg(usrcx, udstx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storeqp(opReg[1], udstx);
				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X to %08X %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(udstx[0]), ZXTL(udstx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTGF:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];

				if ((sts = vaxfp_t::convertgf(usrcx, udstx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Convert %08X %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ZXTL(usrcx[1]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				storel(opReg[2], udstx[0]);
				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Convert %08X %08X to %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCVTFH:
			case OPC_nCVTDH:
			case OPC_nCVTGH:
			case OPC_nCVTHF:
			case OPC_nCVTHD:
			case OPC_nCVTHG:
				throw RSVD_INST_FAULT;


			// **********************
			// F_floaing instructions
			// **********************
			case OPC_nADDF2:
			case OPC_nADDF3:
				usrcx[0] = opReg[0];
				udstx[0] = opReg[1];

				if ((sts = vaxfp_t::addf(usrcx, udstx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X + %08X: %s\n", deviceName(),
							ZXTL(opReg[0]), ZXTL(opReg[1]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storel(opReg[2], uresx[0]);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X + %08X => %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(udstx[0]), ZXTL(uresx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nSUBF2:
			case OPC_nSUBF3:
				usrcx[0] = opReg[0];
				udstx[0] = opReg[1];

				if ((sts = vaxfp_t::subtractf(usrcx, udstx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X - %08X: %s\n", deviceName(),
							ZXTL(opReg[1]), ZXTL(opReg[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storel(opReg[2], uresx[0]);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X - %08X => %08X: %s\n", deviceName(),
						ZXTL(udstx[0]), ZXTL(usrcx[0]), ZXTL(uresx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nMULF2:
			case OPC_nMULF3:
				usrcx[0] = opReg[0];
				udstx[0] = opReg[1];

				if ((sts = vaxfp_t::multiplyf(usrcx, udstx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X + %08X: %s\n", deviceName(),
							ZXTL(opReg[0]), ZXTL(opReg[1]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storel(opReg[2], uresx[0]);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X + %08X => %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(udstx[0]), ZXTL(uresx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nDIVF2:
			case OPC_nDIVF3:
				usrcx[0] = opReg[0];
				udstx[0] = opReg[1];

				if ((sts = vaxfp_t::dividef(usrcx, udstx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X / %08X: %s\n", deviceName(),
							ZXTL(udstx[0]), ZXTL(usrcx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storel(opReg[2], uresx[0]);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X / %08X => %08X: %s\n", deviceName(),
						ZXTL(udstx[0]), ZXTL(usrcx[0]), ZXTL(uresx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nMOVF:
				if ((opReg[0] & UFP_EXP) == 0) {
					if (opReg[0] & UFP_SIGN)
						throw RSVD_OPND_FAULT;
					udstx[0] = 0;
				} else
					udstx[0] = opReg[0];

				storel(opReg[1], udstx[0]);

				// Update condition codes
				fpSetNZ(ccReg, udstx[0], (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move %08X to %08X: %s\n", deviceName(),
						ZXTL(opReg[0]), ZXTL(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nMNEGF:
				// Negate number
				if (opReg[0] & UFP_EXP)
					udstx[0] = opReg[0] ^ UFP_SIGN;
				else {
					if (opReg[0] & UFP_SIGN)
						throw RSVD_OPND_FAULT;
					udstx[0] = 0;
				}
				storel(opReg[1], udstx[0]);

				// Update condition codes
				fpSetNZ(ccReg, SXTL(udstx[0]), 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move/Negate %08X to %08X: %s\n", deviceName(),
						ZXTL(opReg[0]), ZXTL(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nTSTF:
				usrcx[0] = opReg[0];
				if ((usrcx[0] & UFP_EXP) == 0) {
					if (usrcx[0] & UFP_SIGN)
						throw RSVD_OPND_FAULT;
					usrcx[0] = 0;
				}

				// Update condition codes
				fpSetNZ(ccReg, usrcx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Test %08X (%08X): %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(opReg[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCMPF:
				usrcx[0] = opReg[0];
				udstx[0] = opReg[1];

				if ((sts = vaxfp_t::compare(usrcx, udstx, SFP_TYPE, &ccReg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Compare %08X with %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ZXTL(udstx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Compare %08X with %08X: %s\n", deviceName(),
							ZXTL(usrcx[0]), ZXTL(udstx[0]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nACBF:
				udstx[0] = opReg[0];
				usrcx[0] = opReg[1];
				uidx[0]  = opReg[2];

				if ((sts = vaxfp_t::addf(usrcx, uidx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X + %08X: %s\n", deviceName(),
							ZXTL(usrcx[0]), ZXTL(uidx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				if ((sts = vaxfp_t::compare(uresx, udstx, SFP_TYPE, &ccReg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Compare %08X with %08X: %s\n", deviceName(),
								ZXTL(uresx[0]), ZXTL(udstx[0]), ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storel(opReg[3], uresx[0]);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], (ccReg & CC_C));

				if ((ccReg & CC_Z) || ((usrcx[0] & FP_SIGN) ? !(ccReg & CC_N) : (ccReg & CC_N))) {
					REG_PC += opReg[4];
					flushvi();
				}
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND)) {
					const char *jump;
					if ((ccReg & CC_Z) || ((usrcx[0] & FP_SIGN) ? !(ccReg & CC_N) : (ccReg & CC_N)))
						jump = "Jumped";
					else
						jump = "Continue";

					dbg.log("%s: %08X + %08X => %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(uidx[0]), ZXTL(uresx[0]), stringCC(ccReg));
					dbg.log("%s: %08X %s %08X: %s\n", deviceName(),
						ZXTL(udstx[0]), (usrcx[0] & FP_SIGN) ? ">=" : "<=", ZXTL(usrcx[0]), jump);
				}
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nEMODF:
			case OPC_nPOLYF:
				throw RSVD_INST_FAULT;

			// ***********************
			// D_floating instructions
			// ***********************
			case OPC_nADDD2:
			case OPC_nADDD3:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];
				udstx[0] = opReg[2];
				udstx[1] = opReg[3];

				if ((sts = vaxfp_t::addd(usrcx, udstx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X %08X + %08X %08X: %s\n", deviceName(),
							ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
							ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storeqp(opReg[4], uresx);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X %08X + %08X %08X => %08X %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
						ZXTL(uresx[0]), ZXTL(uresx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nSUBD2:
			case OPC_nSUBD3:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];
				udstx[0] = opReg[2];
				udstx[1] = opReg[3];

				if ((sts = vaxfp_t::subtractd(usrcx, udstx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X %08X - %08X %08X: %s\n", deviceName(),
							ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
							ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storeqp(opReg[4], uresx);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X %08X - %08X %08X => %08X %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
						ZXTL(uresx[0]), ZXTL(uresx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nMULD2:
			case OPC_nMULD3:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];
				udstx[0] = opReg[2];
				udstx[1] = opReg[3];

				if ((sts = vaxfp_t::multiplyd(usrcx, udstx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X %08X * %08X %08X: %s\n", deviceName(),
							ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
							ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storeqp(opReg[4], uresx);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X %08X * %08X %08X => %08X %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
						ZXTL(uresx[0]), ZXTL(uresx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nDIVD2:
			case OPC_nDIVD3:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];
				udstx[0] = opReg[2];
				udstx[1] = opReg[3];

				if ((sts = vaxfp_t::divided(usrcx, udstx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X %08X / %08X %08X: %s\n", deviceName(),
							ZXTL(udstx[0]), ZXTL(udstx[1]), ZXTL(usrcx[0]), ZXTL(usrcx[1]),
							ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storeqp(opReg[4], uresx);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X %08X / %08X %08X => %08X %08X: %s\n", deviceName(),
						ZXTL(udstx[0]), ZXTL(udstx[1]), ZXTL(usrcx[0]), ZXTL(usrcx[1]),
						ZXTL(uresx[0]), ZXTL(uresx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nMOVD:
				if ((opReg[0] & UFP_EXP) == 0) {
					if (opReg[0] & UFP_SIGN)
						throw RSVD_OPND_FAULT;
					udstx[0] = udstx[1] = 0;
				} else {
					udstx[0] = opReg[0];
					udstx[1] = opReg[1];
				}

				storeqp(opReg[2], udstx);

				// Update condition codes
				fpSetNZ(ccReg, udstx[0], (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move %08X %08X to %08X %08X: %s\n", deviceName(),
						ZXTL(opReg[0]), ZXTL(opReg[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
						stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nMNEGD:
				if (opReg[0] & UFP_EXP) {
					udstx[0] = opReg[0] ^ UFP_SIGN;
					udstx[1] = opReg[1];
				} else {
					if (opReg[0] & UFP_SIGN)
						throw RSVD_OPND_FAULT;
					udstx[0] = udstx[1] = 0;
				}

				storeqp(opReg[2], udstx);

				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move/Negate %08X %08X to %08X %08X: %s\n", deviceName(),
						ZXTL(opReg[0]), ZXTL(opReg[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
						stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nTSTD:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];

				if ((usrcx[0] & UFP_EXP) == 0) {
					if (usrcx[0] & UFP_SIGN)
						throw RSVD_OPND_FAULT;
					usrcx[0] = usrcx[1] = 0;
				}

				// Update condition codes
				fpSetNZ(ccReg, usrcx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Test %08X %08X (%08X %08X): %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(opReg[0]), ZXTL(opReg[1]),
						stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCMPD:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];
				udstx[0] = opReg[2];
				udstx[1] = opReg[3];

				if ((sts = vaxfp_t::compare(usrcx, udstx, DFP_TYPE, &ccReg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Compare %08X %08X with %08X %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
								ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Compare %08X %08X with %08X %08X: %s\n", deviceName(),
							ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
							stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nACBD:
				udstx[0] = opReg[0];
				udstx[1] = opReg[1];
				usrcx[0] = opReg[2];
				usrcx[1] = opReg[3];
				uidx[0]  = opReg[4];
				uidx[1]  = opReg[5];

				if ((sts = vaxfp_t::addd(usrcx, uidx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X %08X + %08X %08X: %s\n", deviceName(),
							ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(uidx[0]), ZXTL(uidx[1]),
							ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				if ((sts = vaxfp_t::compare(uresx, udstx, DFP_TYPE, &ccReg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Compare %08X %08X with %08X %08X: %s\n", deviceName(),
								ZXTL(uresx[0]), ZXTL(uresx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
								ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storeqp(opReg[6], uresx);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], (ccReg & CC_C));

				if ((ccReg & CC_Z) || ((usrcx[0] & FP_SIGN) ? !(ccReg & CC_N) : (ccReg & CC_N))) {
					REG_PC += opReg[4];
					flushvi();
				}
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND)) {
					const char *jump;
					if ((ccReg & CC_Z) || ((usrcx[0] & FP_SIGN) ? !(ccReg & CC_N) : (ccReg & CC_N)))
						jump = "Jumped";
					else
						jump = "Continue";

					dbg.log("%s: %08X %08X + %08X %08X => %08X %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(uidx[0]), ZXTL(uidx[1]),
						ZXTL(uresx[0]), ZXTL(uresx[1]), stringCC(ccReg));
					dbg.log("%s: %08X %08X %s %08X %08X: %s\n", deviceName(),
						ZXTL(udstx[0]), ZXTL(udstx[1]), (usrcx[0] & FP_SIGN) ? ">=" : "<=",
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), jump);
				}
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nEMODD:
			case OPC_nPOLYD:
				throw RSVD_INST_FAULT;


			// ***********************
			// G_floating instructions
			// ***********************
			case OPC_nADDG2:
			case OPC_nADDG3:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];
				udstx[0] = opReg[2];
				udstx[1] = opReg[3];

				if ((sts = vaxfp_t::addg(usrcx, udstx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X %08X + %08X %08X: %s\n", deviceName(),
							ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
							ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storeqp(opReg[4], uresx);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X %08X + %08X %08X => %08X %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
						ZXTL(uresx[0]), ZXTL(uresx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nSUBG2:
			case OPC_nSUBG3:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];
				udstx[0] = opReg[2];
				udstx[1] = opReg[3];

				if ((sts = vaxfp_t::subtractg(usrcx, udstx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X %08X - %08X %08X: %s\n", deviceName(),
							ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
							ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storeqp(opReg[4], uresx);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X %08X - %08X %08X => %08X %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
						ZXTL(uresx[0]), ZXTL(uresx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nMULG2:
			case OPC_nMULG3:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];
				udstx[0] = opReg[2];
				udstx[1] = opReg[3];

				if ((sts = vaxfp_t::multiplyg(usrcx, udstx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X %08X * %08X %08X: %s\n", deviceName(),
							ZXTL(udstx[0]), ZXTL(udstx[1]), ZXTL(usrcx[0]), ZXTL(usrcx[1]),
							ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storeqp(opReg[4], uresx);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X %08X * %08X %08X => %08X %08X: %s\n", deviceName(),
						ZXTL(udstx[0]), ZXTL(udstx[1]), ZXTL(usrcx[0]), ZXTL(usrcx[1]),
						ZXTL(uresx[0]), ZXTL(uresx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nDIVG2:
			case OPC_nDIVG3:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];
				udstx[0] = opReg[2];
				udstx[1] = opReg[3];

				if ((sts = vaxfp_t::divideg(usrcx, udstx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X %08X / %08X %08X: %s\n", deviceName(),
							ZXTL(udstx[0]), ZXTL(udstx[1]), ZXTL(usrcx[0]), ZXTL(usrcx[1]),
							ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storeqp(opReg[4], uresx);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: %08X %08X / %08X %08X => %08X %08X: %s\n", deviceName(),
						ZXTL(udstx[0]), ZXTL(udstx[1]), ZXTL(usrcx[0]), ZXTL(usrcx[1]),
						ZXTL(uresx[0]), ZXTL(uresx[1]), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nMOVG:
				if ((opReg[0] & GFP_EXP) == 0) {
					if (opReg[0] & GFP_SIGN)
						throw RSVD_OPND_FAULT;
					udstx[0] = udstx[1] = 0;
				} else {
					udstx[0] = opReg[0];
					udstx[1] = opReg[1];
				}

				storeqp(opReg[2], udstx);

				// Update condition codes
				fpSetNZ(ccReg, udstx[0], (ccReg & CC_C));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move %08X %08X to %08X %08X: %s\n", deviceName(),
						ZXTL(opReg[0]), ZXTL(opReg[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
						stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nMNEGG:
				if (opReg[0] & GFP_EXP) {
					udstx[0] = opReg[0] ^ GFP_SIGN;
					udstx[1] = opReg[1];
				} else {
					if (opReg[0] & GFP_SIGN)
						throw RSVD_OPND_FAULT;
					udstx[0] = udstx[0] = 0;
				}

				storeqp(opReg[2], udstx);

				// Update condition codes
				fpSetNZ(ccReg, udstx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Move/Negate %08X %08X to %08X %08X: %s\n", deviceName(),
						ZXTL(opReg[0]), ZXTL(opReg[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
						stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nTSTG:
				srcx[0] = opReg[0];
				srcx[1] = opReg[1];
				if ((srcx[0] & GFP_EXP) == 0) {
					if (srcx[0] & GFP_SIGN)
						throw RSVD_OPND_FAULT;
					srcx[0] = srcx[1] = 0;
				}

				// Update condition codes
				fpSetNZ(ccReg, srcx[0], 0);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Test %08X %08X (%08X %08X): %s\n", deviceName(),
						ZXTL(srcx[0]), ZXTL(srcx[1]), ZXTL(opReg[0]), ZXTL(opReg[1]),
						stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nCMPG:
				usrcx[0] = opReg[0];
				usrcx[1] = opReg[1];
				udstx[0] = opReg[2];
				udstx[1] = opReg[3];

				if ((sts = vaxfp_t::compare(usrcx, udstx, GFP_TYPE, &ccReg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Compare %08X %08X with %08X %08X: %s\n", deviceName(),
								ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
								ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Compare %08X %08X with %08X %08X: %s\n", deviceName(),
							ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
							stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nACBG:
				udstx[0] = opReg[0];
				udstx[1] = opReg[1];
				usrcx[0] = opReg[2];
				usrcx[1] = opReg[3];
				uidx[0]  = opReg[4];
				uidx[1]  = opReg[5];

				if ((sts = vaxfp_t::addg(usrcx, uidx, uresx)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: %08X %08X + %08X %08X: %s\n", deviceName(),
							ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(uidx[0]), ZXTL(uidx[1]),
							ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				if ((sts = vaxfp_t::compare(uresx, udstx, GFP_TYPE, &ccReg)) != VFP_OK) {
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Compare %08X %08X with %08X %08X: %s\n", deviceName(),
								ZXTL(uresx[0]), ZXTL(uresx[1]), ZXTL(udstx[0]), ZXTL(udstx[1]),
								ferrCodes[sts]);
#endif /* ENABLE_DEBUG */
					faultfp(sts);
				}

				// Store results
				storeqp(opReg[6], uresx);
				// Update condition codes
				fpSetNZ(ccReg, uresx[0], (ccReg & CC_C));

				if ((ccReg & CC_Z) || ((usrcx[0] & FP_SIGN) ? !(ccReg & CC_N) : (ccReg & CC_N))) {
					REG_PC += opReg[4];
					flushvi();
				}
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND)) {
					const char *jump;
					if ((ccReg & CC_Z) || ((usrcx[0] & FP_SIGN) ? !(ccReg & CC_N) : (ccReg & CC_N)))
						jump = "Jumped";
					else
						jump = "Continue";

					dbg.log("%s: %08X %08X + %08X %08X => %08X %08X: %s\n", deviceName(),
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), ZXTL(uidx[0]), ZXTL(uidx[1]),
						ZXTL(uresx[0]), ZXTL(uresx[1]), stringCC(ccReg));
					dbg.log("%s: %08X %08X %s %08X %08X: %s\n", deviceName(),
						ZXTL(udstx[0]), ZXTL(udstx[1]), (usrcx[0] & FP_SIGN) ? ">=" : "<=",
						ZXTL(usrcx[0]), ZXTL(usrcx[1]), jump);
				}
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nEMODG:
			case OPC_nPOLYG:
				throw RSVD_INST_FAULT;

			// **********************
			// H_floaing instructions
			// **********************
			case OPC_nADDH2:
			case OPC_nADDH3:
			case OPC_nSUBH2:
			case OPC_nSUBH3:
			case OPC_nMULH2:
			case OPC_nMULH3:
			case OPC_nDIVH2:
			case OPC_nDIVH3:
				throw RSVD_INST_FAULT;

			case OPC_nMOVH:
			case OPC_nTSTH:
			case OPC_nCMPH:
			case OPC_nMNEGH:
			case OPC_nACBH:
			case OPC_nEMODH:
			case OPC_nPOLYH:
				throw RSVD_INST_FAULT;


			// ******************
			// Queue instructions
			// ******************

#define QUE_NEXT(entry) ((entry)+(LN_LONG*0))
#define QUE_PREV(entry) ((entry)+(LN_LONG*1))
#define QUE_HEAD(entry) ((entry)+(LN_LONG*0))
#define QUE_TAIL(entry) ((entry)+(LN_LONG*1))

			case OPC_nINSQUE:
				entry = SXTL(opReg[0]);
				pred  = SXTL(opReg[1]);

				// Check Memory Accesses
				succ = readv(QUE_NEXT(pred), LN_LONG, WACC);
				readv(QUE_PREV(succ), LN_LONG, WACC);
				readv(QUE_PREV(entry), LN_LONG, WACC);

				// Insert an entry into queue.
				// Link entry to current prev and next nodes.
				writev(QUE_NEXT(entry), succ, LN_LONG, WACC);
				writev(QUE_PREV(entry), pred, LN_LONG, WACC);

				// Link prev and next nodes to that entry.
				writev(QUE_PREV(succ), entry, LN_LONG, WACC);
				writev(QUE_NEXT(pred), entry, LN_LONG, WACC);

				// Update Condition Codes
				SetNZ(ccReg, SXTL(succ), SXTL(pred), 0);
				SetC(ccReg, ZXTL(succ), ZXTL(pred));

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Insert %08X to %08X with next %08X: %s\n",
						deviceName(), ZXTL(entry), ZXTL(pred), ZXTL(succ),
						stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nINSQHI:
				entry = SXTL(opReg[0]);
				queue = SXTL(opReg[1]);
				if ((entry == queue) || ((entry|queue) & ~ALIGN_QUAD))
					throw RSVD_OPND_FAULT;
				// Check write access for page faults
				readv(entry, LN_LONG, WACC);
				head = readv(QUE_HEAD(queue), LN_LONG, WACC);

			OPC_INSQHI:
				if (head & 06)
					throw RSVD_OPND_FAULT;
				if (head & 01) {
					ccReg = CC_C;
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Queue %08X is busy (interlocked): %s\n",
							deviceName(), queue, stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				} else {
					// Lock interlock on that queue
					writev(QUE_HEAD(queue), head | 1, LN_LONG, WACC);

					// Get absolute address of node
					head += queue;
					if (probev(head, WACC, &sts) == MM_FAIL)
						writev(QUE_HEAD(queue), head - queue, LN_LONG, WACC);

					// Link entry to the head of queue.
					// Note: implies interlock release.
					writev(QUE_PREV(head),  entry - head,  LN_LONG, WACC);
					writev(QUE_NEXT(entry), head  - entry, LN_LONG, WACC);
					writev(QUE_PREV(entry), queue - entry, LN_LONG, WACC);
					writev(QUE_HEAD(queue), entry - queue, LN_LONG, WACC);

					// Update condition codes
					ccReg = (head == queue) ? CC_Z : 0;

#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Enqueue %08X to queue %08X with first %08X: %s\n", deviceName(),
							entry, queue, head, stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				}
				break;

			case OPC_nINSQTI:
				entry = SXTL(opReg[0]);
				queue = SXTL(opReg[1]);
				if ((entry == queue) || ((entry|queue) & ~ALIGN_QUAD))
					throw RSVD_OPND_FAULT;
				// Check write access for page faults
				readv(entry, LN_LONG, WACC);
				head = readv(QUE_HEAD(queue), LN_LONG, WACC);

				if (head == 0)
					goto OPC_INSQHI;
				if (head & 06)
					throw RSVD_OPND_FAULT;
				if (head & 01) {
					ccReg = CC_C;
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Queue %08X is busy (interlocked): %s\n",
							deviceName(), queue, stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				} else {
					// Lock interlock om that queue.
					writev(QUE_HEAD(queue), head | 1, LN_LONG, WACC);

					// Get tail node from queue and check
					// address for quadword alignment.
					tail = readv(QUE_TAIL(queue), LN_LONG, RACC) + queue;
					if (tail & ~ALIGN_QUAD) {
						writev(QUE_HEAD(queue), head, LN_LONG, WACC);
						throw RSVD_OPND_FAULT;
					}

					// Check Memory Access
					if (probev(tail, WACC, &sts) == MM_FAIL)
						writev(QUE_HEAD(queue), head, LN_LONG, WACC);

					// Link entry to the tail of queue.
					// Note: implies interlock release.
					writev(QUE_NEXT(tail),  entry - tail,  LN_LONG, WACC);
					writev(QUE_NEXT(entry), queue - entry, LN_LONG, WACC);
					writev(QUE_PREV(entry), tail  - entry, LN_LONG, WACC);
					writev(QUE_TAIL(queue), entry - queue, LN_LONG, WACC);

					// Release interlock on that queue.
					writev(QUE_HEAD(queue), head, LN_LONG, WACC);

					// Update Condition Codes
					ccReg = 0;

#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Enqueue %08X to queue %08X with last %08X: %s\n", deviceName(),
							entry, queue, tail, stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				}
				break;

			case OPC_nREMQUE:
				entry = SXTL(opReg[0]);

				// Get prev and next nodes from old entry.
				next = readv(QUE_NEXT(entry), LN_LONG, RACC);
				prev = readv(QUE_PREV(entry), LN_LONG, RACC);

				// Update Condition Codes
				SetNZ(ccReg, SXTL(next), SXTL(prev), 0);
				SetC(ccReg, ZXTL(next), ZXTL(prev));

				if (entry != prev) {
					// Check Memory Access
					readv(QUE_PREV(next), LN_LONG, WACC);
					if (!OP_ISREG(opReg[1]))
						readv(opReg[1], LN_LONG, WACC);

					// Remove entry from queue.
					writev(QUE_NEXT(prev), next, LN_LONG, WACC);
					writev(QUE_PREV(next), prev, LN_LONG, WACC);
				} else
					ccReg |= CC_V;

				// Put entry into register/memory.
				storel(opReg[1], entry);

#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Remove %08X from %08X with next %08X: %s\n",
						deviceName(), ZXTL(entry), ZXTL(pred), ZXTL(succ),
						stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				break;

			case OPC_nREMQHI:
				queue = SXTL(opReg[0]);
				if (queue & ~ALIGN_QUAD)
					throw RSVD_OPND_FAULT;
				if (!OP_ISREG(opReg[1])) {
					if (opReg[1] == queue)
						throw RSVD_OPND_FAULT;
					readv(opReg[1], LN_LONG, WACC);
				}
				head = readv(QUE_HEAD(queue), LN_LONG, WACC);

			OPC_REMQHI:
				if (head & 06)
					throw RSVD_OPND_FAULT;
				if (head & 01) {
					ccReg = CC_V|CC_C;
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Queue %08X is busy (interlocked): %s\n",
							deviceName(), queue, stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				} else if (head != 0) {
					// Lock interlock on that queue.
					writev(QUE_HEAD(queue), head | 1, LN_LONG, WACC);
					entry = head + queue;
					if (probev(entry, WACC, &sts) == MM_FAIL)
						writev(QUE_HEAD(queue), head, LN_LONG, WACC);

					// Get next node from head node.
					next = readv(QUE_NEXT(entry), LN_LONG, RACC) + entry;
					if (next & ~ALIGN_QUAD) {
						writev(QUE_HEAD(queue), head, LN_LONG, WACC);
						throw RSVD_OPND_FAULT;
					}
					if (probev(next, WACC, &sts) == MM_FAIL)
						writev(QUE_HEAD(queue), head, LN_LONG, WACC);

					// Remove first entry from head of queue.
					// Note: Implies interlock release.
					writev(QUE_PREV(next),  queue - next,  LN_LONG, WACC);
					writev(QUE_HEAD(queue), next  - queue, LN_LONG, WACC);

					// Put dequeued entry into operand #1.
					storel(opReg[1], entry);

					// Update Condition Codes
					ccReg = (next == queue) ? CC_Z : 0;

#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Dequeue %08X from queue %08X with first %08X: %s\n",
							deviceName(), entry, queue, head, stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				} else {
					// Put empty first entry.
					storel(opReg[1], (head + queue));

					// Update Condition Codes.
					ccReg = CC_Z|CC_V;

#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Queue %08X is already empty: %s\n",
							deviceName(), queue, stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				}
				break;

			case OPC_nREMQTI:
				queue = SXTL(opReg[0]);
				if (queue & ~ALIGN_QUAD)
					throw RSVD_OPND_FAULT;
				if (!OP_ISREG(opReg[1])) {
					if (opReg[1] == queue)
						throw RSVD_OPND_FAULT;
					readv(opReg[1], LN_LONG, WACC);
				}
				head = readv(queue, LN_LONG, WACC);
#ifdef ENABLE_DEBUG
				if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
					dbg.log("%s: Head of queue %08X => %08X\n",
						deviceName(), queue, head);
#endif /* ENABLE_DEBUG */

				if (head & 06)
					throw RSVD_OPND_FAULT;
				if (head & 01) {
					ccReg = CC_V|CC_C;
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Queue %08X is busy (interlocked): %s\n",
							deviceName(), queue, stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				} else if (head != 0) {
					// Lock interlock om that queue.
					writev(QUE_HEAD(queue), head | 1, LN_LONG, WACC);

					// Get last entry from tail of queue.
					entry = readv(QUE_TAIL(queue), LN_LONG, RACC);
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Tail of queue %08X => %08X (%08X)\n",
							deviceName(), queue, entry, queue + entry);
#endif /* ENABLE_DEBUG */

					if (entry == head) {
						writev(QUE_HEAD(queue), head, LN_LONG, WACC);
						goto OPC_REMQHI;
					}
					if (entry & ~ALIGN_QUAD) {
						writev(QUE_HEAD(queue), head, LN_LONG, WACC);
						throw RSVD_OPND_FAULT;
					}
					entry += queue;
					if (probev(QUE_PREV(entry), RACC, &sts) == MM_FAIL)
						writev(QUE_HEAD(queue), head, LN_LONG, WACC);

					// Get previous node from head node.
					prev = readv(QUE_PREV(entry), LN_LONG, RACC) + entry;
#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Previous of node %08X => %08X (%08X)\n",
							deviceName(), entry, prev - entry, prev);
#endif /* ENABLE_DEBUG */

					if (prev & ~ALIGN_QUAD) {
						writev(QUE_HEAD(queue), head, LN_LONG, WACC);
						throw RSVD_OPND_FAULT;
					}
					if (probev(prev, WACC, &sts) == MM_FAIL)
						writev(QUE_HEAD(queue), head, LN_LONG, WACC);

					// Remove last entry from head of queue.
					writev(QUE_NEXT(prev),  queue - prev,  LN_LONG, WACC);
					writev(QUE_TAIL(queue), prev  - queue, LN_LONG, WACC);

					// Release interlock on that queue.
					writev(QUE_HEAD(queue), head, LN_LONG, WACC);

					// Put dequeued entry into operand #1.
					storel(opReg[1], entry);

					// Update Condition Codes
					ccReg = 0;

#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Dequeue %08X from queue %08X with last %08X: %s\n",
							deviceName(), entry, queue, head, stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				} else {
					// Queue already is empty, so that can put
					// default queue into operand #1.
					storel(opReg[1], queue);

					// Update Condition Codes.
					ccReg = CC_Z|CC_V;

#ifdef ENABLE_DEBUG
					if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
						dbg.log("%s: Queue %08X is already empty: %s\n",
							deviceName(), queue, stringCC(ccReg));
#endif /* ENABLE_DEBUG */
				}
				break;

			// *******************
			// String instructions
			// *******************

			// MOVC3 - Move characters (3 registers)
			// MOVC5 - Move characters (5 registers)
			case OPC_nMOVC3:
				movc(0);
				break;
			case OPC_nMOVC5:
				movc(1);
				break;

			// CMPC3 - Compare characters (3 registers)
			// CMPC5 - Compare characters (5 registers)
			case OPC_nCMPC3:
				cmpc(0);
				break;
			case OPC_nCMPC5:
				cmpc(1);
				break;

			// LOCC - Locate character
			// SKPC - skip character
			case OPC_nLOCC:
				locc(false);
				break;
			case OPC_nSKPC:
				locc(true);
				break;

			// SCANC - scan character
			// SPANC - span character
			case OPC_nSCANC:
				scanc(false);
				break;
			case OPC_nSPANC:
				scanc(true);
				break;

			// ****************
			// CIS instructions
			// ****************

			case OPC_nMOVP:
			case OPC_nADDP4:  case OPC_nADDP6:
			case OPC_nSUBP4:  case OPC_nSUBP6:
			case OPC_nMULP6:  case OPC_nDIVP6:
			case OPC_nCMPP3:  case OPC_nCMPP4:
			case OPC_nCVTPL:  case OPC_nCVTLP:
			case OPC_nCVTPS:  case OPC_nCVTSP:
			case OPC_nCVTPT:  case OPC_nCVTTP:
			case OPC_nASHP:   case OPC_nCRC:
			case OPC_nMOVTC:  case OPC_nMOVTUC:
			case OPC_nMATCHC: case OPC_nEDITPC:
				emulate(opCode);
				break;

			// context switch instructions
			// LDPCTX - Load process
			// SVPCTX - Save process
			case OPC_nLDPCTX:
				ldpctx();
				break;
			case OPC_nSVPCTX:
				svpctx();
				break;

			// Change access instructions
			// CHMU - Change mode to user mode
			// CHMS - Change mode to supervisor mode
			// CHME - Change mode to executive mode
			// CHMK - Change mode to kernel mode
			case OPC_nCHMU:
				change(AM_USER, SXTW(opReg[0]));
				break;
			case OPC_nCHMS:
				change(AM_SUPERVISOR, SXTW(opReg[0]));
				break;
			case OPC_nCHME:
				change(AM_EXECUTIVE, SXTW(opReg[0]));
				break;
			case OPC_nCHMK:
				change(AM_KERNEL, SXTW(opReg[0]));
				break;

			// Probe instructions
			// PROBER - Probe read access
			// PROBEW - Probe write access
			case OPC_nPROBER:
				ccReg = probe(false) | (ccReg & CC_C);
				break;
			case OPC_nPROBEW:
				ccReg = probe(true) | (ccReg & CC_C);
				break;

			// Unimplemented instruction
			default:
				throw STOP_UOPC;
			}
		}

		catch (int32_t exCode) {
			// Exception fault codes
//			if (exCode >= 0)
//				exCode = fault(exCode);
			if (exCode >= 0)
				scbCode = exCode;

			// Stop codes
			if (exCode < 0) {
				state = cpuStopped;
				switch (exCode) {
				case STOP_HALT:
					printf("%s: %s at PC %08X\n", deviceName(), stopNames[~exCode], faultAddr);
					return;

				case STOP_UOPC:
					printf("%s: Opcode %s - Unimplemented opcode at PC %08X\n",
							deviceName(), dopc->opCode->opName, faultAddr);
					return;

				case STOP_INIE:
					printf("%s: Exception during exception at PC %08X\n",
							deviceName(), faultAddr);
					return;

				case STOP_ILLVEC:
					printf("%s: Illegal vector at PC %08X\n",
							deviceName(), faultAddr);
					return;

				case STOP_OPR:
					printf("%s: Operator request at PC %08X\n",
							deviceName(), faultAddr);
					return;
				}
			}
		}
	}
}
