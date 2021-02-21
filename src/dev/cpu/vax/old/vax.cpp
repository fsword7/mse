/*
 * vax.cpp
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 */

#include "emu/emucore.h"
#include "dev/cpu/vax/mtpr.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/fpu.h"
#include "dev/cpu/vax/opcodes.h"

const uint32_t vax_cpu_base::mskList[] = {
	0x00000000,
	0x00000001, 0x00000003, 0x00000007, 0x0000000F,
	0x0000001F, 0x0000003F, 0x0000007F, 0x000000FF,
	0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF,
	0x00001FFF, 0x00003FFF, 0x00007FFF, 0x0000FFFF,
	0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000FFFFF,
	0x001FFFFF, 0x003FFFFF, 0x007FFFFF, 0x00FFFFFF,
	0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF, 0x0FFFFFFF,
	0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF
};

const uint32_t vax_cpu_base::sgnList[] = {
	0x00000000,
	0x00000001, 0x00000002, 0x00000004, 0x00000008,
	0x00000010, 0x00000020, 0x00000040, 0x00000080,
	0x00000100, 0x00000200, 0x00000400, 0x00000800,
	0x00001000, 0x00002000, 0x00004000, 0x00008000,
	0x00010000, 0x00020000, 0x00040000, 0x00080000,
	0x00100000, 0x00200000, 0x00400000, 0x00800000,
	0x01000000, 0x02000000, 0x04000000, 0x08000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000
};

vax_cpu_base::vax_cpu_base(const system_config &config, const device_type_base &type,
		tag_t *tag, device_t *owner, uint64_t clock, uint8_t dWidth, uint8_t aWidth)
: cpu_device(config, type, tag, owner, clock),
  mapProgramConfig("program", endianLittle, dWidth, aWidth)
{
	buildOpcodes();
//	reset();
}

vax_cpu_base::~vax_cpu_base()
{
}

di_memory::spaceConfigVector vax_cpu_base::mapGetSpaceVector() const
{
	return spaceConfigVector {
		std::make_pair(AS_PROGRAM, &mapProgramConfig)
	};
}

//void vax_cpu_base::reset()
//{
//	// Initialize all working registers
//	for (int idx = 0; idx < VAX_nGREGS; idx++)
//		gRegs[idx].l = 0;
//}

//int  vax_cpu_base::boot()
//{
//	return 0;
//}

void vax_cpu_base::init()
{
	// Initialize address space
	mapProgram = &getAddressSpace(AS_PROGRAM);
}

void vax_cpu_base::devStart()
{
	init();
}

void vax_cpu_base::devStop()
{
}

void vax_cpu_base::devReset()
{
}

//void vax_cpu_base::send(cpuSignal signal)
//{
//	switch (signal) {
//	case cpuStop:
//		state = cpuStopping;
//		break;
//	}
//}

void vax_cpu_base::assignMemory(uint8_t *mem, uint32_t memSize)
{
	this->mem     = mem;
	this->memSize = memSize;
}

void vax_cpu_base::setPCAddr(uint32_t pcAddr)
{
	REG_PC = pcAddr;
}

uint32_t vax_cpu_base::readpr(uint32_t)
{
	return 0;
}

void vax_cpu_base::writepr(uint32_t, uint32_t)
{
}

void vax_cpu_base::check(uint32_t code)
{
}

char *vax_cpu_base::stringCC(uint32_t cc)
{
	static char ccstr[5];

	ccstr[0] = (cc & CC_N) ? 'N' : '-';
	ccstr[1] = (cc & CC_Z) ? 'Z' : '-';
	ccstr[2] = (cc & CC_V) ? 'V' : '-';
	ccstr[3] = (cc & CC_C) ? 'C' : '-';
	ccstr[4] = 0;

	return ccstr;
}

// For CALLG/CALLS instruction
void vax_cpu_base::call(bool stkFlag)
{
	uint32_t npc = ZXTL(opReg[1]);
	uint32_t mask, entry;
	uint32_t tsp;
	int      stkSize;

	mask = readv(npc, LN_WORD, RACC);
	if (mask & CALL_MBZ)
		throw RSVD_OPND_FAULT;

	// Check write access for page faults
	stkSize = stkFlag ? 24 : 20;
	for (int idx = REG_nR0; idx <= REG_nR11; idx++)
		if ((mask >> idx) & 1)
			stkSize += LN_WORD;
	readv(REG_SP - stkSize, LN_BYTE, WACC);

	// Save registers into stack
	if (stkFlag) {
		writev(REG_SP - LN_LONG, opReg[0], LN_LONG, WACC);
		REG_SP -= LN_LONG;
	}
	entry = ((REG_SP & CALL_SPA) << CALL_P_SPA) |
		    ((stkFlag ? 1u : 0u) << CALL_P_S) |
		    ((psReg|ccReg) & CALL_PSW) |
		    ((mask & CALL_MASK) << CALL_P_MASK);
	tsp = REG_SP & ~CALL_SPA;
	for (int idx = REG_nR11; idx >= REG_nR0; idx--) {
		if ((mask >> idx) & 1) {
			writev(tsp - LN_LONG, gpReg[idx].l, LN_LONG, WACC);
			tsp -= LN_LONG;
		}
	}
	writev(tsp - LN_LONG, REG_PC, LN_LONG, WACC);
	writev(tsp - (LN_LONG*2), REG_FP, LN_LONG, WACC);
	writev(tsp - (LN_LONG*3), REG_AP, LN_LONG, WACC);
	writev(tsp - (LN_LONG*4), entry, LN_LONG, WACC);
	writev(tsp - (LN_LONG*5), 0, LN_LONG, WACC);

	// Set new registers
	REG_AP = stkFlag ? REG_SP : opReg[0];
	REG_SP = tsp - (LN_LONG*5);
	REG_FP = REG_SP;
	REG_PC = npc + LN_WORD;
	psReg = (psReg & ~(PSW_DV|PSW_FU|PSW_IV)) |
			((mask & CALL_DV) ? PSW_DV : 0) |
			((mask & CALL_IV) ? PSW_IV : 0);
	ccReg = 0;

	// Clear instruction look-ahead
	flushvi();
}

// For RET instruction
void vax_cpu_base::ret()
{
	uint32_t entry, mask;
	uint32_t npc;
	uint32_t tsp = REG_FP;
	int      stkSize;

	// Get entry mask from stack
	entry = readv(tsp + LN_LONG, LN_LONG, RACC);
	if (entry & CALL_MBZ)
		throw RSVD_OPND_FAULT;
	mask = (entry >> CALL_P_MASK) & CALL_MASK;

	// Check read access for page faults
	stkSize = (entry & CALL_S) ? 23 : 19;
	for (int idx = REG_nR0; idx <= REG_nR11; idx++)
		if ((mask >> idx) & 1)
			stkSize += LN_LONG;
	readv(REG_SP + stkSize - 1, LN_BYTE, RACC);

	// Restore PC, FP, AP, and SP registers
	REG_AP = readv(tsp + (LN_LONG*2), LN_LONG, RACC);
	REG_FP = readv(tsp + (LN_LONG*3), LN_LONG, RACC);
	npc    = readv(tsp + (LN_LONG*4), LN_LONG, RACC);
	tsp   += LN_LONG*5;

	// Restore registers from stack
	for (int idx = REG_nR0; idx <= REG_nR11; idx++) {
		if ((mask >> idx) & 1) {
			gpReg[idx].l = readv(tsp, LN_LONG, RACC);
			tsp += LN_LONG;
		}
	}

	// Dealign stack pointer
	REG_SP = tsp + ((entry >> CALL_P_SPA) & CALL_SPA);

	// Pop old argument if CALLS instruction is used
	if (entry & CALL_S) {
		uint32_t tmp = readv(REG_SP, LN_LONG, RACC);
		REG_SP += ((tmp & MSK_BYTE) << 2) + LN_LONG;
	}

	// Reset PSW register
	psReg = (psReg & ~PSW_MASK) | (entry & (PSW_MASK & ~PSW_CC));
	ccReg = entry & PSW_CC;

	// Set new PC register and clear instruction look-ahead
	REG_PC = npc;
	flushvi();
}

void vax_cpu_base::ldpctx()
{
	uint32_t npc, npsl;
	uint32_t pcb, data;

	// Must be in kernel mode
	if (PSL_GETCUR(psReg) != AM_KERNEL)
		throw PRIV_INST_FAULT;

	// Restore all registers from PCB block
	pcb = IPR_PCBB & ALIGN_LONG;
	IPR_KSP = readpl(pcb);
	IPR_ESP = readpl(pcb+4);
	IPR_SSP = readpl(pcb+8);
	IPR_USP = readpl(pcb+12);
	REG_R0  = readpl(pcb+16);
	REG_R1  = readpl(pcb+20);
	REG_R2  = readpl(pcb+24);
	REG_R3  = readpl(pcb+28);
	REG_R4  = readpl(pcb+32);
	REG_R5  = readpl(pcb+36);
	REG_R6  = readpl(pcb+40);
	REG_R7  = readpl(pcb+44);
	REG_R8  = readpl(pcb+48);
	REG_R9  = readpl(pcb+52);
	REG_R10 = readpl(pcb+56);
	REG_R11 = readpl(pcb+60);
	REG_R12 = readpl(pcb+64);
	REG_R13 = readpl(pcb+68);
	npc     = readpl(pcb+72);
	npsl    = readpl(pcb+76);

	// Load P0BR register
	data = readpl(pcb+80);
	if (IPR_BR_TEST(data))
		throw RSVD_OPND_FAULT;
	IPR_P0BR = data & BR_MASK;

	// Load P0LR register
	data = readpl(pcb+84);
	if (LP_P0LR_TEST(data))
		throw RSVD_OPND_FAULT;
	if (IPR_LR_TEST(data & LR_MASK))
		throw RSVD_OPND_FAULT;
	IPR_P0LR = data & LR_MASK;

	// Load ASTLVL register
	data = (data >> 24) & AST_MASK;
	if (LP_AST_TEST(data))
		throw RSVD_OPND_FAULT;
	IPR_ASTLVL = data;

	// Load P1BR register
	data = readpl(pcb+88);
	if (IPR_BR_TEST(data + 0x800000))
		throw RSVD_OPND_FAULT;
	IPR_P1BR = data & BR_MASK;

	// Load P1LR register
	data = readpl(pcb+92);
	if (LP_P1LR_TEST(data))
		throw RSVD_OPND_FAULT;
	if (IPR_LR_TEST(data & LR_MASK))
		throw RSVD_OPND_FAULT;
	IPR_P1LR = data & LR_MASK;

	// Load PME register
	IPR_PME = (data >> 31) & 1;

	// Flush all process table entries
	cleartlb(false);

	// Switch to kernel mode
	if ((psReg & PSL_IS) != 0) {
		psReg &= ~PSL_IS;
		IPR_ISP = REG_SP;
		REG_SP  = IPR_KSP;
	}

	// Push new PC and PSL into stack for REI instruction
	REG_SP -= (LN_LONG*2);
	writev(REG_SP, npc, LN_LONG, WACC);
	writev(REG_SP+LN_LONG, npsl, LN_LONG, WACC);

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND)) {
		dbg.log("%s: (CTX) Loading User Process\n", deviceName());
		dbg.log("%s: (CTX) PCB Address: %08X, AST Level: %d, PME: %s\n", deviceName(),
			IPR_PCBB, IPR_ASTLVL, (IPR_PME & 1) ? "On" : "Off");
		dbg.log("%s: (CTX) KSP: %08X  ESP: %08X  SSP: %08X  USP: %08X\n", deviceName(),
			IPR_KSP, IPR_ESP, IPR_SSP, IPR_USP);
		dbg.log("%s: (CTX) P0 Address: %08X Length: %08X\n", deviceName(),
			IPR_P0BR, IPR_P0LR);
		dbg.log("%s: (CTX) P1 Address: %08X Length: %08X\n", deviceName(),
			IPR_P1BR, IPR_P1LR);
		dbg.log("%s: (CTX) SP %08X <= PC %08X PSL %08X\n", deviceName(),
			REG_SP, npc, npsl);
	}
#endif /* ENABLE_DEBUG */
}

void vax_cpu_base::svpctx()
{
	uint32_t svpc, svpsl;
	uint32_t pcb, osp;

	// Must be in kernel mode
	if (PSL_GETCUR(psReg) != AM_KERNEL)
		throw PRIV_INST_FAULT;

	// Get saved PC and PSL registers from stack
	osp     = REG_SP;
	svpc    = readv(REG_SP, LN_LONG, RACC);
	svpsl   = readv(REG_SP+LN_LONG, LN_LONG, RACC);
	REG_SP += (LN_LONG*2);

	// Switch to interrupt mode with IPL 1
	if ((psReg & PSL_IS) == 0) {
		IPR_KSP = REG_SP;
		REG_SP  = IPR_ISP;
		if (PSL_GETIPL(psReg) == 0)
			psReg |= PSL_SETIPL(1);
		psReg |= PSL_IS;
	}

	// Save all registers into PCB block
	pcb = IPR_PCBB & ALIGN_LONG;
	writepl(pcb,    IPR_KSP);
	writepl(pcb+4,  IPR_ESP);
	writepl(pcb+8,  IPR_SSP);
	writepl(pcb+12, IPR_USP);
	writepl(pcb+16, REG_R0);
	writepl(pcb+20, REG_R1);
	writepl(pcb+24, REG_R2);
	writepl(pcb+28, REG_R3);
	writepl(pcb+32, REG_R4);
	writepl(pcb+36, REG_R5);
	writepl(pcb+40, REG_R6);
	writepl(pcb+44, REG_R7);
	writepl(pcb+48, REG_R8);
	writepl(pcb+52, REG_R9);
	writepl(pcb+56, REG_R10);
	writepl(pcb+60, REG_R11);
	writepl(pcb+64, REG_R12);
	writepl(pcb+68, REG_R13);
	writepl(pcb+72, svpc);
	writepl(pcb+76, svpsl);

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND)) {
		dbg.log("%s: (CTX) Saving User Process\n", deviceName());
		dbg.log("%s: (CTX) PCB Address: %08X, AST Level: %d, PME: %s\n", deviceName(),
			IPR_PCBB, IPR_ASTLVL, (IPR_PME & 1) ? "On" : "Off");
		dbg.log("%s: (CTX) KSP: %08X  ESP: %08X  SSP: %08X  USP: %08X\n", deviceName(),
			IPR_KSP, IPR_ESP, IPR_SSP, IPR_USP);
		dbg.log("%s: (CTX) P0 Address: %08X Length: %08X\n", deviceName(),
			IPR_P0BR, IPR_P0LR);
		dbg.log("%s: (CTX) P1 Address: %08X Length: %08X\n", deviceName(),
			IPR_P1BR, IPR_P1LR);
		dbg.log("%s: (CTX) SP %08X => PC %08X PSL %08X\n", deviceName(),
			osp, svpc, svpsl);
	}
#endif /* ENABLE_DEBUG */
}

uint32_t vax_cpu_base::probe(bool rwflg)
{
	uint32_t mode  = opReg[0] & ACC_MASK;
	uint32_t len   = opReg[1];
	uint32_t base  = opReg[2];
	uint32_t pmode = PSL_GETPRV(ccReg);
	uint32_t acc, sts;

	if (pmode > mode)
		mode = pmode;
	acc = AM_MASK(mode) << (rwflg ? TLB_P_WRACC : TLB_P_RDACC);

	// Check virtual access at start of address
	probev(base, acc, &sts);

	switch (sts) {
	case MM_TNV:
	case MM_OK:
		break;
	case MM_PTNV:
		paCount  = 2;
		paReg[0] = (rwflg ? MM_WRITE : 0) | (sts & MM_EMASK);
		paReg[1] = base;
		throw PAGE_TNV;
	default:
		return CC_Z;
	}

	// Check virtual access at end of address
	probev(base + len - 1, acc, &sts);

	switch (sts) {
	case MM_TNV:
	case MM_OK:
		break;
	case MM_PTNV:
		paCount  = 2;
		paReg[0] = (rwflg ? MM_WRITE : 0) | (sts & MM_EMASK);
		paReg[1] = base + len - 1;
		throw PAGE_TNV;
	default:
		return CC_Z;
	}

	// All probe attempts are successful.
	return 0;
}

int vax_cpu_base::getBit()
{
	int32_t  pos = opReg[0];
	int32_t  reg = opReg[1];
	uint32_t ea, src;

	if (OP_ISREG(reg)) {
		if (ZXTL(pos) > 31)
			throw RSVD_OPND_FAULT;
		src = gpReg[~reg].l;
#ifdef ENABLE_DEBUG
		if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
			dbg.log("%s: R%d %08X<%d> => %d\n", deviceName(),
				reg, src, pos, (src >> pos) & 1);
#endif /* ENABLE_DEBUG */
	} else {
		ea   = opReg[1] + (pos >> 3);
		src  = readv(ea, LN_BYTE, RACC);
		pos &= 7;
#ifdef ENABLE_DEBUG
		if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
			dbg.log("%s: %08X => %02X<%d> => %d\n", deviceName(),
				ea, ZXTB(src), pos, (src >> pos) & 1);
#endif /* ENABLE_DEBUG */
	}

	return (src >> pos) & 1;
}

int vax_cpu_base::setBit(int bit)
{
	int32_t  pos = opReg[0];
	int32_t  reg = opReg[1];
	uint32_t ea, src, dst;
	int      obit;

	if (OP_ISREG(reg)) {
		if (ZXTL(pos) > 31)
			throw RSVD_OPND_FAULT;
		src  = gpReg[~reg].l;
		obit = (src >> pos) & 1;
		dst  = bit ? (src | (1u << pos)) : (src & ~(1u << pos));
		gpReg[~reg].l = dst;
#ifdef ENABLE_DEBUG
		if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
			dbg.log("%s: R%d %08X<%d> (now: %08X) <= %d (old: %d)\n",
				deviceName(), reg, src, dst, pos, bit, obit);
#endif /* ENABLE_DEBUG */
	} else {
		ea   = opReg[1] + (pos >> 3);
		src  = readv(ea, LN_BYTE, RACC);
		pos &= 7;
		obit = (src >> pos) & 1;
		dst  = bit ? (src | (1u << pos)) : (src & ~(1u << pos));
		writev(ea, dst, LN_BYTE, WACC);
#ifdef ENABLE_DEBUG
		if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
			dbg.log("%s: %08X => %02X<%d> (now: %02X) <= %d (old: %d)\n",
				deviceName(), ea, ZXTB(src), ZXTB(dst), pos, bit, obit);
#endif /* ENABLE_DEBUG */
	}

	return obit;
}

int32_t vax_cpu_base::getField(bool sign)
{
	int32_t  pos  = SXTL(opReg[0]);
	uint8_t  size = ZXTB(opReg[1]);
	uint32_t reg  = ZXTL(opReg[2]);
	uint32_t src1, src2, ea;

	// If size is zero, do nothing and return zero.
	if (size == 0)
		return 0;
	if (size > 32)
		throw RSVD_OPND_FAULT;

	// Extract a field from one or two longwords.
	if (OP_ISREG(reg)) {
		if (ZXTL(pos) > 31)
			throw RSVD_OPND_FAULT;
//		if (~reg >= REG_nSP)
//			throw RSVD_ADDR_FAULT;
		src1 = ZXTL(gpReg[~reg].l);
		if ((pos + size) > 32)
			src2 = ZXTL(gpReg[(~reg)+1].l);
	} else {
		ea   = reg + (pos >> 3);
		pos  = (pos & 07) | ((ea & 03) << 3);
		ea  &= ~03;
		src1 = readv(ea, LN_LONG, RACC);
		if ((pos + size) > 32)
			src2 = readv(ea+LN_LONG, LN_LONG, RACC);
	}
	if (pos > 0)
		src1 = (src1 >> pos) | (src2 << (32-pos));

	// Zero or sign extension
//	printf("%s: %d => %08X %08X\n", deviceName(),
//		size, ZXTL(MSK_LONG << size), (1u << size-1));

//	src1 &= ~ZXTL(MSK_LONG << size);
//	if (sign && (src1 & (1u << size-1)))
//		src1 |= ZXTL(MSK_LONG << size);
	src1 &= mskList[size];
	if (sign && (src1 & sgnList[size]))
		src1 |= ~mskList[size];

	// Return result
	return src1;
}

void vax_cpu_base::putField()
{
	uint32_t src  = ZXTL(opReg[0]);
	int32_t  pos  = SXTL(opReg[1]);
	uint8_t  size = ZXTB(opReg[2]);
	uint32_t reg  = ZXTL(opReg[3]);
	uint32_t src1, src2;
	uint32_t dst1, dst2;
	uint32_t ea, mask;

	// If size is zero, do nothing and return zero.
	if (size == 0)
		return;

	// If size is more than 32, reserved operand fault.
	if (size > 32)
		throw RSVD_OPND_FAULT;

	// Extract a field from one or two longwords.
	if (OP_ISREG(reg)) {
		if (ZXTL(pos) > 31)
			throw RSVD_OPND_FAULT;
		if (ZXTL(pos + size) > 32) {
//			if (~reg >= REG_nSP)
//				throw RSVD_ADDR_FAULT;
			mask = mskList[pos + size - 32];
			src2 = gpReg[(~reg)+1].l;
			dst2 = ((src >> (32 - pos)) & mask) | (src2 & ~mask);
			gpReg[(~reg)+1].l = dst2;
		}
		src1 = gpReg[~reg].l;
		mask = mskList[size] << pos;
		dst1 = ((src << pos) & mask) | (src1 & ~mask);
		gpReg[~reg].l = dst1;
	} else {
		ea   = opReg[3] + (pos >> 3);
		pos  = (pos & 07) | ((ea & 03) << 3);
		ea  &= ~03;
		src1 = readv(ea, LN_LONG, RACC);
		if ((pos + size) > 32) {
			src2 = readv(ea+LN_LONG, LN_LONG, RACC);
			mask = mskList[pos + size - 32];
			dst2 = ((src >> (32 - pos)) & mask) | (src2 & ~mask);
			writev(ea+LN_LONG, dst2, LN_LONG, WACC);
		}
		mask = mskList[size] << pos;
		dst1 = ((src << pos) & mask) | (src1 & ~mask);
		writev(ea, dst1, LN_LONG, WACC);
//		printf("%s: Mask=%08X Pos=%d Size=%d\n", deviceName(),
//			mask, pos, size);
//		printf("%s: %08X => %08X\n", deviceName(), src1, dst1);
	}
}


static const char *ieNames[]  = { "Interrupt", "Exception", "Severe Exception" };
static const char *ieTypes[]  = { "INT", "EXC", "SVE" };
static const char *accNames[] = { "Kernel", "Executive", "Supervisor", "User" };

// Arithmetic Exception Trap/Fault Names
static const char *trpNames[] =
{
	"Integer Overflow Trap",
	"Integer Divide-by-Zero Trap",
	"Floating Overflow Trap",
	"Floating/Decimal Divide-by-Zero Trap",
	"Floating Underflow Trap",
	"Decimal Overflow Trap",
	"Subscript Range Trap",
	"Floating Overflow Fault",
	"Floating Divide Fault",
	"Floating Underflow Fault"
};

#define DSPL_CUR(mode) accNames[PSL_GETCUR(mode)]
#define DSPL_PRV(mode) accNames[PSL_GETPRV(mode)]

static const uint32_t swMasks[IPL_SMAX] =
{
	0xFFFE, 0xFFFC, 0xFFF8, 0xFFF0, // IPL 00 - 03
	0xFFE0, 0xFFC0, 0xFF80, 0xFF00, // IPL 04 - 07
	0xFE00, 0xFC00, 0xF800, 0xF000, // IPL 08 - 0B
	0xE000, 0xC000, 0x8000          // IPL 0C - 0E
};

void vax_cpu_base::halt(uint32_t code)
{
	printf("%s: (DIE) Exception during exception at PC %08X\n",
		deviceName(), faultAddr);
	throw STOP_INIE;
}

// Evaluate IRQ levels
int vax_cpu_base::evaluate()
{
	uint32_t ipl = PSL_GETIPL(psReg);

	// Evaluate software interrupt requests
	if (ipl >= IPL_SMAX)
		return 0;
	if (IPR_SISR & swMasks[ipl]) {
		for (int sidx = IPL_SMAX; sidx > 0; sidx--) {
			if (IPR_SISR & swMasks[sidx-1])
				return sidx;
		}
	}

	// No interrupt pending
	return 0;
}


void vax_cpu_base::interrupt()
{
	uint32_t nipl, trap;
	uint32_t vec;

	if ((trap = IRQ_GETTRAP(irqFlags)) != 0) {
#ifdef ENABLE_DEBUG
		if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND) || dbg.checkFlags(DBG_EXCEPTION))
			dbg.log("%s: (EXC) Arithmetic Trap at PC %08X: %s\n",
				deviceName(), faultAddr, trpNames[trap-1]);
#endif /* ENABLE_DEBUG */
		// Set parameters
		paCount  = 1;
		paReg[0] = trap;
		// perform exception routine
		exception(IE_EXC, SCB_ARITH, 0);
	} else if ((nipl = IRQ_GETIPL(irqFlags)) != 0) {
//		if (nipl == IPL_HALTPIN) {
//			halt(HALT_PIN);
//		}
		if (nipl <= IPL_SMAX) {
			// Software interrupts
			IPR_SISR &= ~(1u << nipl);
			vec = SCB_IPLSOFT + (nipl << 2);
		} else
			// Undefined IPL level
			throw STOP_UIPL;

		// perform exception routine
		exception(IE_INT, vec, nipl);
	} else
		irqFlags = 0;
}

void vax_cpu_base::faultfp(uint32_t fsts)
{
	switch (fsts) {
	case VFP_FAULT:
		throw RSVD_OPND_FAULT;

	case VFP_DZRO: // Floating divide by Zero error
		paCount  = 1;
		paReg[0] = FAULT_FLTDIV;
		throw ARITH_FAULT;

	case VFP_OVFL: // Floating overflow error
		paCount  = 1;
		paReg[0] = FAULT_FLTOVF;
		throw ARITH_FAULT;

	case VFP_UNFL: // Floating underflow error
		if (psReg & PSW_FU) {
			paCount  = 1;
			paReg[0] = FAULT_FLTUND;
			throw ARITH_FAULT;
		}
		return;
	}
}

int vax_cpu_base::fault(uint32_t vec)
{
	int rc;

	// Reset registers that already
	// were incremented or decremented.
	if ((psReg & PSL_FPD) == 0)
		for (int idx = 0; idx < rqCount; idx++)
			gpReg[rqReg[idx] & 0xF].l += rqReg[idx] >> 4;

	// Clear TP in PSL register
	psReg &= ~PSL_TP;

	// Reset fault PC address
	REG_PC = faultAddr;

	switch (vec) {
	case SCB_RESIN|SCB_NOPRIV:
		vec &= ~SCB_NOPRIV;
	case SCB_RESAD:
	case SCB_RESOP:
	case SCB_RESIN:
		if (flags & CPU_INIE)
			halt(HALT_ISNV);
		if ((rc = exception(IE_EXC, vec, 0)) != 0)
			return rc;
		break;

//	case SCB_CMODE:
	case SCB_ARITH:
		if (flags & CPU_INIE)
			halt(HALT_ISNV);
		exception(IE_EXC, vec, 0);
		break;

	case SCB_TNV: // Translation Invalid
	case SCB_ACV: // Access Control Violation
//		if (flags & CPU_INIE) {
//			if (psReg & PSL_IS) {
//				printf("%s: (DIE) Page fault during interrupt mode at PC %08X\n",
//						deviceName(), faultAddr);
//				throw STOP_INIE;
//			}
//			// Kernel Stack Not Valid
//			paCount = 0;
//			exception(IE_SVE, SCB_KSNV, 0);
//		} else
//			exception(IE_EXC, vec, 0);
		if (flags & CPU_INIE)
			halt(HALT_ISNV);
		exception(IE_EXC, vec, 0);
		break;

	case SCB_KSNV: // Kernel Stack Not Valid
		if (psReg & PSL_IS)
			halt(HALT_ISNV);
		exception(IE_SVE, vec, 0);
		break;

	case SCB_MCHK: // Machine Check (model-specific)
		if (flags & CPU_INIE)
			halt(HALT_MCHK);
		check(paReg[1]);
		break;

	case SCB_NEXT: // Passive release - continue
		break;
	}

	return 0;
}

int vax_cpu_base::exception(int ie, uint32_t vec, uint32_t ipl)
{
	uint32_t opsl = psReg|ccReg;
	uint32_t opc  = faultAddr;
	uint32_t osp  = REG_SP;
	uint32_t prv  = PSL_GETCUR(psReg);
	uint32_t npc, npsl;
	uint32_t tsp;

	// Set INIE flag
	flags |= CPU_INIE;

	// Clear all traps
	irqFlags &= ~IRQ_TRAP;

	// Get SCBB vector from memory
	npc = readpl(IPR_SCBB + vec);
	if (ie == IE_SVE)
		npc |= 1;
	if (npc & 2)
		halt((npc & 1) ? HALT_SCB11 : HALT_SCB10);

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND) || dbg.checkFlags(DBG_EXCEPTION))
		dbg.log("%s: (%s) SCB vector %04X  New PC: %08X(%1X) Type: %s\n", deviceName(),
			ieTypes[ie], ZXTW(vec), ZXTL(npc & ~03), ZXTL(npc & 03), ieNames[ie]);
#endif /* ENABLE_DEBUG */

	if (npc & 2)
		throw STOP_ILLVEC;

	// Switch SP registers
	if (opsl & PSL_IS)
		npsl = PSL_IS;
	else {
		ipReg[prv] = REG_SP;
		if (npc & 1) {
			npsl   = PSL_IS;
			REG_SP = IPR_ISP;
		} else {
			npsl   = 0;
			REG_SP = IPR_KSP;
		}
	}

	// Set new PSL register
	if (ie == IE_INT) {
		psReg = npsl | PSL_SETIPL(ipl);
	} else {
		psReg = npsl | ((npc & 1) ? PSL_IPL : (opsl & PSL_IPL)) |
			PSL_SETPRV(prv);
	}
	ccReg = 0;

	// Save old PC, PSL and parameters in kernel mode
	curMode = AM_MASK(AM_KERNEL);
	tsp = REG_SP;
	writev((tsp -= LN_LONG), opsl, LN_LONG, WACC);
	writev((tsp -= LN_LONG), opc,  LN_LONG, WACC);
	if (paCount > 0) {
		for (int idx = paCount-1; idx >= 0; idx--)
			writev((tsp -= LN_LONG), paReg[idx], LN_LONG, WACC);
		paCount = 0;
	}
	REG_SP = tsp;

	// Set new PC address and reset opcode stream
	REG_PC = npc & SCB_ADDR;
	flushvi();

	// Set new access mode
	curMode = AM_MASK(PSL_GETCUR(psReg));

	// Clear INIE flag
	flags &= ~CPU_INIE;

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND) || dbg.checkFlags(DBG_EXCEPTION)) {
		dbg.log("%s: (%s) Old PC=%08X PSL=%08X SP=%08X Access: %s,%s\n", deviceName(),
			ieTypes[ie], ZXTL(faultAddr), ZXTL(opsl), ZXTL(osp), DSPL_CUR(opsl), DSPL_PRV(opsl));
		dbg.log("%s: (%s) New PC=%08X PSL=%08X SP=%08X Access: %s,%s\n", deviceName(),
			ieTypes[ie], ZXTL(REG_PC), ZXTL(psReg|ccReg), ZXTL(REG_SP), DSPL_CUR(npsl), DSPL_PRV(npsl));
	}
#endif /* ENABLE_DEBUG */

	return 0;
}

// Emulate the instruction
void vax_cpu_base::emulate(uint32_t opCode)
{
	uint32_t vec, npc;
	uint32_t opc, osp, opsl;

	opc  = faultAddr;
	osp  = REG_SP;
	opsl = psReg|ccReg;

	if ((psReg & PSL_FPD) == 0) {
		vec = SCB_EMULATE;
		// Save operand registers into stack
		writev(REG_SP - (LN_LONG*12), opCode, LN_LONG, WACC);
		writev(REG_SP - (LN_LONG*11), faultAddr, LN_LONG, WACC);
		writev(REG_SP - (LN_LONG*10), opReg[0], LN_LONG, WACC);
		writev(REG_SP - (LN_LONG*9),  opReg[1], LN_LONG, WACC);
		writev(REG_SP - (LN_LONG*8),  opReg[2], LN_LONG, WACC);
		writev(REG_SP - (LN_LONG*7),  opReg[3], LN_LONG, WACC);
		writev(REG_SP - (LN_LONG*6),  opReg[4], LN_LONG, WACC);
		writev(REG_SP - (LN_LONG*5),  opReg[5], LN_LONG, WACC);
		writev(REG_SP - (LN_LONG*2),  REG_PC, LN_LONG, WACC);
		writev(REG_SP - (LN_LONG*1),  psReg|ccReg, LN_LONG, WACC);
		REG_SP -= 48;
	} else {
		vec = SCB_EMULFPD;
		writev(REG_SP - (LN_LONG*2),  faultAddr, LN_LONG, WACC);
		writev(REG_SP - (LN_LONG*1),  psReg|ccReg, LN_LONG, WACC);
		REG_SP -= 8;
	}

	// Get new PC address from SCB block
	npc = readpl((IPR_SCBB + vec) & ALIGN_LONG);

	// Clear some PSL registers
	psReg &= ~(PSL_TP|PSL_FPD|PSW_DV|PSW_FU|PSW_IV|PSW_T);
	ccReg  = 0;

	// Set PC address and clear instruction look-ahead
	REG_PC = npc & SCB_ADDR;
	flushvi();

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND) || dbg.checkFlags(DBG_EXCEPTION)) {
		dbg.log("%s: (EMU) Emulated instruction (opcode %04X) at PC %08X\n", deviceName(),
			opCode + ((opCode > OPC_nXFC) ? (OPC_nXFC << 8) : 0), opc);
		dbg.log("%s: (EMU) SCB vector %04X  New PC: %08X(%1X) Type: Emulate\n", deviceName(),
			ZXTW(vec), ZXTL(npc & ~03), ZXTL(npc & 03));
		dbg.log("%s: (EMU) Old PC=%08X PSL=%08X SP=%08X Access: %s,%s\n", deviceName(),
			ZXTL(opc), ZXTL(opsl), ZXTL(osp), DSPL_CUR(opsl), DSPL_PRV(opsl));
		dbg.log("%s: (EMU) New PC=%08X PSL=%08X SP=%08X Access: %s,%s\n", deviceName(),
			ZXTL(REG_PC), ZXTL(psReg|ccReg), ZXTL(REG_SP), DSPL_CUR(psReg), DSPL_PRV(psReg));
	}
#endif /* ENABLE_DEBUG */
}

static const uint32_t scbMode[] = { SCB_CHMK, SCB_CHME, SCB_CHMS, SCB_CHMU };

// Change access mode for CHMx instructions
void vax_cpu_base::change(int mode, int32_t code)
{
	uint32_t omode, nmode;
	uint32_t opc, opsl, osp;
	uint32_t npc, nsp;

	// Save PC, PSL and SP registers
	opsl = psReg|ccReg;
	opc  = REG_PC;
	osp  = REG_SP;

	// Must not be in interrupt mode
	if (psReg & PSL_IS)
		halt(HALT_CHMFIS);

	// Get vector address from SCB block
	npc = readpl(IPR_SCBB + scbMode[mode]);

	// Maximize access mode
	omode = PSL_GETPRV(psReg);
	nmode = (omode < mode) ? omode : mode;

	// Switch SP register
	CPU_IPR(omode) = osp;
	nsp = CPU_IPR(nmode);

	// Set new access mode
	curMode = AM_MASK(nmode);

	writev((nsp -= LN_LONG), opsl, LN_LONG, WACC);
	writev((nsp -= LN_LONG), opc, LN_LONG, WACC);
	writev((nsp -= LN_LONG), code, LN_LONG, WACC);
	REG_SP = nsp;

	// Load new PC and PSL in new access mode
	psReg = (psReg & PSL_IPL) | PSL_SETCUR(nmode) | PSL_SETPRV(omode);
	ccReg = 0;
	REG_PC = npc;
	flushvi();

	// Evaluate hardware/software interrupts

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND) || dbg.checkFlags(DBG_EXCEPTION)) {
		dbg.log("%s: (CHM) SCB vector %04X  New PC: %08X(%1X) Type: Change Mode\n", deviceName(),
			ZXTW(scbMode[mode]), ZXTL(npc & ~03), ZXTL(npc & 03));
		dbg.log("%s: (CHM) Old PC=%08X PSL=%08X SP=%08X Access: %s,%s\n", deviceName(),
			ZXTL(opc), ZXTL(opsl), ZXTL(osp), DSPL_CUR(opsl), DSPL_PRV(opsl));
		dbg.log("%s: (CHM) New PC=%08X PSL=%08X SP=%08X Access: %s,%s\n", deviceName(),
			ZXTL(REG_PC), ZXTL(psReg|ccReg), ZXTL(REG_SP), DSPL_CUR(psReg), DSPL_PRV(psReg));
	}
#endif /* ENABLE_DEBUG */
}

// Resume from exception/interrupt routine
void vax_cpu_base::resume()
{
	uint32_t npc, npsl;
	uint32_t opc, opsl, osp;
	uint32_t nacc, oacc;
	uint32_t nipl, oipl;

	opc  = REG_PC;
	opsl = psReg|ccReg;
	osp  = REG_SP;

	// Get PC and PSL from kernel/interrupt stack
	npc  = readv(REG_SP, LN_LONG, RACC);
	npsl = readv(REG_SP+LN_LONG, LN_LONG, RACC);

	// Get access mode from old and new PSL register
	nacc = PSL_GETCUR(npsl);
	oacc = PSL_GETCUR(psReg);
	oipl = PSL_GETIPL(psReg);

	// Check validation against MBZ and access modes
	if ((npsl & PSL_MBZ) || (nacc < oacc))
		throw RSVD_OPND_FAULT;
	if (nacc == AM_KERNEL) {
		// Check validation for kernel mode
		nipl = PSL_GETIPL(npsl);
		if ((npsl & PSL_IS) && (((opsl & PSL_IS) == 0) || (nipl == 0)))
			throw RSVD_OPND_FAULT;
		if (nipl > PSL_GETIPL(opsl))
			throw RSVD_OPND_FAULT;
	} else {
		// Check validation for non-kernel mode
		if ((npsl & (PSL_IS|PSL_IPL)) || nacc > PSL_GETPRV(npsl))
			throw RSVD_OPND_FAULT;
	}
	// Check compatibility mode
	if (npsl & PSL_CM)
		throw RSVD_OPND_FAULT;

	// All validation check passed...
	REG_SP += (LN_LONG*2);

	// Save current SP register
	if (psReg & PSL_IS)
		IPR_ISP = REG_SP;
	else
		ipReg[oacc] = REG_SP;

	// Set new PSL register
	psReg = (psReg & PSL_TP) | (npsl & ~PSW_CC);
	ccReg = npsl & PSW_CC;

	// Set new PC register and reset opcode stream
	REG_PC = npc;
	flushvi();

	// Set new SP register for new access mode
	// Also check AST delivery request
	if ((psReg & PSL_IS) == 0) {
		REG_SP = ipReg[nacc];
		// Request AST delivery (software IPL 2)
		if (nacc >= IPR_ASTLVL)
			IPR_SISR |= SISR_2;
	}

	// Update current access mode
	curMode = AM_MASK(PSL_GETCUR(psReg));

	// Update IRQ requests
	UpdateIRQ();

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND) || dbg.checkFlags(DBG_EXCEPTION)) {
		dbg.log("%s: (REI) Old PC=%08X PSL=%08X SP=%08X Access: %s,%s\n", deviceName(),
			ZXTL(opc), ZXTL(opsl), ZXTL(osp), DSPL_CUR(opsl), DSPL_PRV(opsl));
		dbg.log("%s: (REI) New PC=%08X PSL=%08X SP=%08X Access: %s,%s\n", deviceName(),
			ZXTL(REG_PC), ZXTL(psReg|ccReg), ZXTL(REG_SP), DSPL_CUR(npsl), DSPL_PRV(npsl));
		if ((psReg & PSL_IS) == 0 && nacc >= IPR_ASTLVL)
			dbg.log("%s: (REI) AST delivered (%d >= %d)\n", deviceName(),
				nacc, IPR_ASTLVL);
	}
#endif /* ENABLE_DEBUG */
}
