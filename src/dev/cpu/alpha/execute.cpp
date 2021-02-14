/*
 * execute.cpp - Alpha CPU Processor package - execute routines
 *
 *  Created on: Nov 21, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/alpha/axp.h"
#include "dev/cpu/alpha/debug.h"
#include "dev/cpu/alpha/opcode.h"

// Instruction defines
#include "dev/cpu/alpha/axp_arith.h"
#include "dev/cpu/alpha/axp_bwx.h"
#include "dev/cpu/alpha/axp_logical.h"
#include "dev/cpu/alpha/axp_branch.h"
#include "dev/cpu/alpha/axp_mem.h"
#include "dev/cpu/alpha/axp_misc.h"
#include "dev/cpu/alpha/axp_pal.h"

#include "dev/cpu/alpha/axp_fpbr.h"
#include "dev/cpu/alpha/axp_fpmem.h"
#include "dev/cpu/alpha/axp_fpoper.h"


void alpha_cpuDevice::init()
{

	// Clear all regular/shadow registers
	for (int idx = 0; idx < AXP_NIREGS*2; idx++)
		state.iRegs[idx] = 0;
	for (int idx = 0; idx < AXP_NFREGS*2; idx++)
		state.fRegs[idx] = 0;

	// Clear all instruction cache
	for (int idx = 0; idx < ICACHE_ENTRIES; idx++)
	{
		state.iCache[idx].valid = false;
		state.iCache[idx].asn = 0;
		state.iCache[idx].asmb = false;
		state.iCache[idx].vAddr = 0;
		state.iCache[idx].pAddr = 0;
		for (int didx = 0; didx < ICACHE_LINE_SIZE; didx++)
			state.iCache[idx].data[didx] = 0;
	}
	state.iCacheLast   = 0;
	state.iCacheNext   = 0;
	state.iCacheEnable = true;

	// Clear all translation buffers
	for (int tbidx = 0; tbidx < 2; tbidx++)
	{
		for (int eidx = 0; eidx < TB_ENTRIES; eidx++)
		{
			state.tb[tbidx][eidx].valid = false;
			state.tb[tbidx][eidx].asmb  = false;
			state.tb[tbidx][eidx].asn   = 0;
			state.tb[tbidx][eidx].matchMask = 0;
			state.tb[tbidx][eidx].fault[0] = 0;
			state.tb[tbidx][eidx].fault[1] = 0;
			state.tb[tbidx][eidx].fault[2] = 0;
			state.tb[tbidx][eidx].access[0][0] = 0;
			state.tb[tbidx][eidx].access[1][0] = 0;
			state.tb[tbidx][eidx].access[0][1] = 0;
			state.tb[tbidx][eidx].access[1][1] = 0;
			state.tb[tbidx][eidx].access[0][2] = 0;
			state.tb[tbidx][eidx].access[1][2] = 0;
			state.tb[tbidx][eidx].access[0][3] = 0;
			state.tb[tbidx][eidx].access[1][3] = 0;
			state.tb[tbidx][eidx].keepMask = 0;
			state.tb[tbidx][eidx].vAddr = 0;
			state.tb[tbidx][eidx].pAddr = 0;
		}
		state.tbLast[tbidx][0] = 0;
		state.tbLast[tbidx][1] = 0;
	}

	state.icm = ACC_KERNEL;
	state.asn = 0;
	state.fpen = 1;

	// Clear all interrupt enables
	state.eien = 0;
	state.sien = 0;
	state.asten = 0;
	state.cren = 0;
	state.slen = 0;

	// Interrupt priority order
	state.iplr = 0;
	state.aster = 0;

	// Clear all interrupt requests
	state.eir = 0;
	state.sir = 0;
	state.astrr = 0;
	state.crr = 0;
	state.slr = 0;
	state.intid = 0;

	// Clear TB registers
	state.asn0 = 0;
	state.asn1 = 0;
	state.dcm = ACC_KERNEL;
	state.altcm = ACC_KERNEL;

	// procesor-specific reset
	preset();

	mapProgram = getAddressSpace(AS_PROGRAM);
}

void alpha_cpuDevice::setPCAddress(offs_t addr)
{
	state.pcAddr = addr;
}

void alpha_cpuDevice::step(Console *user)
{
	// Save current log flags and
	// enable console output
	uint32_t flags = dbg.getLogFlags();
	dbg.setLogFlags(LOG_CONSOLE);

	// Execute one instruction as single step
	log = user;
	execute();

	// Restore log flags
	dbg.loadLogFlags(flags);
}

void alpha_cpuDevice::run()
{
	// Start execution state
	pState = execRunning;

	try {
		while(pState == execRunning)
		{
			execute();
		}
		dbg.flushAll();
	}

	catch (...)
	{
		// Flush all remaining buffers
		dbg.flushAll();
	}

	// Stop execution state
	pState = execStopped;
}

int alpha_cpuDevice::fetchi(uint64_t addr, uint32_t &data)
{
	int      idx = state.iCacheLast;
	bool     asmBit = false;
	uint64_t vAddr, pAddr;

	if (state.iCacheEnable == true)
	{
		vAddr = addr & ICACHE_MATCH_MASK;

		// instruction cache hit
		if (state.iCache[idx].valid &&
			(state.iCache[idx].asn == state.asn || state.iCache[idx].asmb) &&
			(state.iCache[idx].vAddr == vAddr))
		{
			data = state.iCache[idx].data[(addr >> 2) & ICACHE_INDEX_MASK];
			return 0;
		}

		// instruction cache search
		for (idx = 0; idx < ICACHE_ENTRIES; idx++)
		{
			if (state.iCache[idx].valid &&
				(state.iCache[idx].asn == state.asn || state.iCache[idx].asmb) &&
				(state.iCache[idx].vAddr == vAddr))
			{
				state.iCacheLast = idx;
				data = state.iCache[idx].data[(addr >> 2) & ICACHE_INDEX_MASK];
				return 0;
			}
		}

		if (vAddr & 1)
		{
			pAddr  = vAddr & ~0x1ull;
			asmBit = true;
		}
		else
		{
//			pAddr = probev(vAddr, ACC_EXEC, 0, asmBit, status);
//			if (status != 0)
//				return status;
			pAddr  = vAddr & ~0x1ull;
		}

		// Instruction cache miss - setup now
		idx = state.iCacheNext++;
		if (state.iCacheNext == ICACHE_ENTRIES)
			state.iCacheNext = 0;

		state.iCache[idx].valid = true;
		state.iCache[idx].asn   = state.asn;
		state.iCache[idx].asmb  = asmBit;
		state.iCache[idx].vAddr = vAddr;
		state.iCache[idx].pAddr = pAddr;

		// Fetch 2048-byte data block from memeory
		mapProgram->readBlock(state.iCache[idx].pAddr,
			(uint8_t *)&state.iCache[idx].data, ICACHE_LINE_SIZE*4);

		state.iCacheLast = idx;
		data = state.iCache[idx].data[(addr >> 2) & ICACHE_INDEX_MASK];
	}

	// Instruction cache disabled

	data = mapProgram->read32(state.pcAddr, this);
	return 0;
}

void alpha_cpuDevice::execute()
{
	int opCode;
	uint32_t func;
	uint64_t pAddr;

	uint64_t dbgval;
	string   dbgstr;

	// For debugging at this time.
	// To be removed later...
	if (((state.pcAddr & ~0x3ull) == 0xC000) ||
		((state.pcAddr & ~0x3ull) == 0x8000))
	{
		dbg.setDebugFlags(DBG_TRACE);
		fmt::printf("%s: Starting trace at %X\n",
			getDeviceName(), state.pcAddr);
		cout << flush;
	}

	// Count cycle if enable
	if (state.cc_ena)
		state.cc++;


//	{
//		// Check any external interrupts, software interrupts and AST interrupts
//		if ((state.eien & state.eir) || (state.sien & state.sir) ||
//			(state.asten & (state.aster & state.astrr & ((1 << (state.icm  + 1)) - 1))))
//		{
//			enterPALMode(EXC_INTERRUPT);
//		}
//	}

	// Display current instruction
//	list(nullptr, state.pcAddr);

	state.fpcAddr = state.pcAddr;
//	opWord = readv32(state.pcAddr);
	fetchi(state.pcAddr, state.opWord);
	nextPC();

	// R31/F31 register - always zero
	state.iRegs[REG_ZERO] = 0;
	state.fRegs[REG_ZERO] = 0;

	opCode = OP_GETOP(state.opWord);
	switch (opCode)
	{
	case OPC_PAL:		// 00 - CALL_PAL instruction
		OPC_FUNC(CALL_PAL, call_pal, PAL);
		break;

	case OPC_LDA:		// 08 - LDA instruction
		OPC_EXEC(LDA, MEM);
		break;

	case OPC_LDAH:		// 09 - LDAH instruction
		OPC_EXEC(LDAH, MEM);
		break;

	case OPC_LDBU:		// 0A - LDBU instruction
		OPC_EXEC(LDBU, MEM);
		break;

	case OPC_LDQ_U:		// 0B - LDQ_U instruction
		OPC_EXEC(LDQ_U, MEM);
		break;

	case OPC_LDWU:		// 0C - LDWU instruction
		OPC_EXEC(LDWU, MEM);
		break;

	case OPC_STW:		// 0D - STW instruction
		OPC_EXEC(STW, MEMS);
		break;

	case OPC_STB:		// 0E - STB instruction
		OPC_EXEC(STB, MEMS);
		break;

	case OPC_STQ_U:		// OF - STQ_U instruction
		OPC_EXEC(STQ_U, MEMS);
		break;

	case OPC_INTA:		// 10 - Arithmetic instructions
		func = (state.opWord >> 5) & 0x7F;
		switch (func)
		{
		case 0x00:
			OPC_EXEC(ADDL, RAB_RC);
			break;
		case 0x02:
			OPC_EXEC(S4ADDL, RAB_RC);
			break;
		case 0x12:
			OPC_EXEC(S8ADDL, RAB_RC);
			break;
//		case 0x40:
//			OPC_EXEC(ADDL_V, RAB_RC);
//			break;

		case 0x20:
			OPC_EXEC(ADDQ, RAB_RC);
			break;
		case 0x22:
			OPC_EXEC(S4ADDQ, RAB_RC);
			break;
		case 0x32:
			OPC_EXEC(S8ADDQ, RAB_RC);
			break;
//		case 0x60:
//			OPC_EXEC(ADDQ_V, RAB_RC);
//			break;


		case 0x09:
			OPC_EXEC(SUBL, RAB_RC);
			break;
		case 0x0B:
			OPC_EXEC(S4SUBL, RAB_RC);
			break;
		case 0x1B:
			OPC_EXEC(S8SUBL, RAB_RC);
			break;
//		case 0x49:
//			OPC_EXEC(SUBL_V, RAB_RC);
//			break;

		case 0x29:
			OPC_EXEC(SUBQ, RAB_RC);
			break;
		case 0x2B:
			OPC_EXEC(S4SUBQ, RAB_RC);
			break;
		case 0x3B:
			OPC_EXEC(S8SUBL, RAB_RC);
			break;
//		case 0x69:
//			OPC_EXEC(SUBQ_V, RAB_RC);
//			break;

//		case 0x0F:
//			OPC_EXEC(CMPBGE, RAB_RC);
//			break;

		case 0x1D:
			OPC_EXEC(CMPULT, RAB_RC);
			break;
		case 0x2D:
			OPC_EXEC(CMPEQ, RAB_RC);
			break;
		case 0x3D:
			OPC_EXEC(CMPULE, RAB_RC);
			break;
		case 0x4D:
			OPC_EXEC(CMPLT, RAB_RC);
			break;
		case 0x6D:
			OPC_EXEC(CMPLE, RAB_RC);
			break;

		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_INTL:		// 11 - Logical instructions
		func = (state.opWord >> 5) & 0x7F;
		switch (func)
		{
		case 0x00:
			OPC_EXEC(AND, RAB_RC);
			break;
		case 0x08:
			OPC_EXEC(BIC, RAB_RC);
			break;
		case 0x14:
			OPC_EXEC(CMOVLBS, RAB_RC);
			break;
		case 0x16:
			OPC_EXEC(CMOVLBC, RAB_RC);
			break;
		case 0x20:
			OPC_EXEC(BIS, RAB_RC);
			break;
		case 0x24:
			OPC_EXEC(CMOVEQ, RAB_RC);
			break;
		case 0x26:
			OPC_EXEC(CMOVNE, RAB_RC);
			break;
		case 0x28:
			OPC_EXEC(ORNOT, RAB_RC);
			break;
		case 0x40:
			OPC_EXEC(XOR, RAB_RC);
			break;
		case 0x44:
			OPC_EXEC(CMOVLT, RAB_RC);
			break;
		case 0x46:
			OPC_EXEC(CMOVGE, RAB_RC);
			break;
		case 0x48:
			OPC_EXEC(EQV, RAB_RC);
			break;
		case 0x61:
			OPC_EXEC(AMASK, RB_RC);
			break;
		case 0x64:
			OPC_EXEC(CMOVLE, RAB_RC);
			break;
		case 0x66:
			OPC_EXEC(CMOVGT, RAB_RC);
			break;
		case 0x6C:
			OPC_EXEC(IMPLVER, X_RC);
			break;
		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_INTS:		// 12 - Shift instructions
		func = (state.opWord >> 5) & 0x7F;
		switch (func)
		{
	    case 0x02:
	      OPC_EXEC(MSKBL, RAB_RC);
	      break;
	    case 0x06:
	      OPC_EXEC(EXTBL, RAB_RC);
	      break;
	    case 0x0B:
	      OPC_EXEC(INSBL, RAB_RC);
	      break;
	    case 0x12:
	      OPC_EXEC(MSKWL, RAB_RC);
	      break;
	    case 0x16:
	      OPC_EXEC(EXTWL, RAB_RC);
	      break;
	    case 0x1B:
	      OPC_EXEC(INSWL, RAB_RC);
	      break;
	    case 0x22:
	      OPC_EXEC(MSKLL, RAB_RC);
	      break;
	    case 0x26:
	      OPC_EXEC(EXTLL, RAB_RC);
	      break;
	    case 0x2b:
	      OPC_EXEC(INSLL, RAB_RC);
	      break;
	    case 0x30:
	      OPC_EXEC(ZAP, RAB_RC);
	      break;
	    case 0x31:
	      OPC_EXEC(ZAPNOT, RAB_RC);
	      break;
	    case 0x32:
	      OPC_EXEC(MSKQL, RAB_RC);
	      break;
	    case 0x34:
	      OPC_EXEC(SRL, RAB_RC);
	      break;
	    case 0x36:
	      OPC_EXEC(EXTQL, RAB_RC);
	      break;
	    case 0x39:
	      OPC_EXEC(SLL, RAB_RC);
	      break;
	    case 0x3B:
	      OPC_EXEC(INSQL, RAB_RC);
	      break;
	    case 0x3C:
	      OPC_EXEC(SRA, RAB_RC);
	      break;
	    case 0x52:
	      OPC_EXEC(MSKWH, RAB_RC);
	      break;
	    case 0x57:
	      OPC_EXEC(INSWH, RAB_RC);
	      break;
	    case 0x5A:
	      OPC_EXEC(EXTWH, RAB_RC);
	      break;
	    case 0x62:
	      OPC_EXEC(MSKLH, RAB_RC);
	      break;
	    case 0x67:
	      OPC_EXEC(INSLH, RAB_RC);
	      break;
	    case 0x6A:
	      OPC_EXEC(EXTLH, RAB_RC);
	      break;
	    case 0x72:
	      OPC_EXEC(MSKQH, RAB_RC);
	      break;
	    case 0x77:
	      OPC_EXEC(INSQH, RAB_RC);
	      break;
	    case 0x7a:
	      OPC_EXEC(EXTQH, RAB_RC);
	      break;
		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_INTM:		// 13 - Multiply instructions
		func = (state.opWord >> 5) & 0x7F;
		switch (func)
		{
		case 0x00:
			OPC_EXEC(MULL, RAB_RC);
			break;
		case 0x20:
			OPC_EXEC(MULQ, RAB_RC);
			break;
//		case 0x40:
//			OP_EXEC2(MULL_V, RAB_RC);
//			break;
//		case 0x60:
//			OP_EXEC2(MULQ_V, RAB_RC);
//			break;
//		case 0x30:
//			OP_EXEC2(UMULH, RAB_RC);
//			break;
		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_ITFP:		// 14 - Integer/Floating instructions
		func = (state.opWord >> 5) & 0x7FF;
		switch (func)
		{
		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_FLTV:		// 15 - Floating VAX instructions
		func = (state.opWord >> 5) & 0x7FF;
		switch (func)
		{
		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_FLTI:		// 16 - Floating IEEE instructions
		func = (state.opWord >> 5) & 0x7FF;
		switch (func)
		{
		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_FLTL:		// 17 - FLTL instruction
		func = (state.opWord >> 5) & 0x7FF;
		switch (func)
		{
		case 0x24: // MT_FPCR
			OPC_EXEC(MT_FPCR, X_FA);
			break;

		case 0x25: // MF_FPCR
			OPC_EXEC(MF_FPCR, X_FA);
			break;

		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_MISC:		// 18 - Miscellaneous instructions
		func = state.opWord & 0xFFFF;
		switch (func)
		{
		case 0x0000: // 0000 - TRAPB
			OPC_EXEC(TRAPB, NOP);
			break;
		case 0x0400: // EXCB
			OPC_EXEC(EXCB, NOP);
			break;
		case 0x4000: // MB
			OPC_EXEC(MB, NOP);
			break;
		case 0x4400: // WMB
			OPC_EXEC(WMB, NOP);
			break;
		case 0x8000: // FETCH
			OPC_EXEC(FETCH, NOP);
			break;
		case 0xA000: // FETCH_M
			OPC_EXEC(FETCH_M, NOP);
			break;
		case 0xC000: // RPCC
			OPC_EXEC(RPCC, X_RA);
			break;
		case 0xE000: // RC
			OPC_EXEC(RC_, X_RA);
			break;
		case 0xE800: // ECB
			OPC_EXEC(ECB, NOP);
			break;
		case 0xF000: // RS
			OPC_EXEC(RS, X_RA);
			break;
		case 0xF800: // WH64
			OPC_EXEC(WH64, NOP);
			break;
		case 0xFC00: // WH64EN
			OPC_EXEC(WH64EN, NOP);
			break;
		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_HW_MFPR:	// 19 - HW_MFPR instruction
		OPC_FUNC(HW_MFPR, hw_mfpr, MFPR);
		break;

	case OPC_JSR:		// 1A - JSR instruction
		OPC_EXEC(JMP, JMP);
		break;

	case OPC_HW_LD:		// 1B - HW_LD instruction
		func = (state.opWord >> 12) & 0xF;
		if (func & 1) {
			OPC_EXEC(HW_LDQ, HW_LD);
		} else {
			OPC_EXEC(HW_LDL, HW_LD);
		}
		break;

	case OPC_FPTI:		// 1C - Floating/Integer instructions
		func = (state.opWord >> 5) & 0x7F;
		switch (func)
		{
		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_HW_MTPR:	// 1D - HW_MTPR instruction
		OPC_FUNC(HW_MTPR, hw_mtpr, MTPR);
		break;

	case OPC_RET:		// 1E - HW_REI/HW_RET instruction
		OPC_FUNC(HW_REI, hw_ret, RET);
		break;

	case OPC_HW_ST:		// 1F - HW_ST instruction
		func = (state.opWord >> 12) & 0xF;
		if (func & 1) {
			OPC_EXEC(HW_STQ, HW_ST);
		} else {
			OPC_EXEC(HW_STL, HW_ST);
		}
		break;

	case OPC_LDF:		// 20 - LDF instruction
		OPC_EXEC(LDF, FMEM);
		break;

	case OPC_LDG:		// 21 - LDG instruction
		OPC_EXEC(LDG, FMEM);
		break;

	case OPC_LDS:		// 22 - LDS instruction
		OPC_EXEC(LDS, FMEM);
		break;

	case OPC_LDT:		// 23 - LDT instruction
		OPC_EXEC(LDT, FMEM);
		break;

	case OPC_STF:		// 24 - STF instruction
		OPC_EXEC(STF, FMEMS);
		break;

	case OPC_STG:		// 25 - STG instruction
		OPC_EXEC(STG, FMEMS);
		break
		;
	case OPC_STS:		// 26 - STS instruction
		OPC_EXEC(STS, FMEMS);
		break;

	case OPC_STT:		// 27 - STT instruction
		OPC_EXEC(STT, FMEMS);
		break;

	case OPC_LDL:		// 28 - LDL instruction
		OPC_EXEC(LDL, MEM);
		break;

	case OPC_LDQ:		// 29 - LDQ instruction
		OPC_EXEC(LDQ, MEM);
		break;

	case OPC_LDL_L:		// 2A - LDL_L instruction
		OPC_EXEC(LDL_L, MEM);
		break;

	case OPC_LDQ_L:		// 2B - LDQ_L instruction
		OPC_EXEC(LDQ_L, MEM);
		break;

	case OPC_STL:		// 2C - STL instruction
		OPC_EXEC(STL, MEMS);
		break;

	case OPC_STQ:		// 2D - STQ instruction
		OPC_EXEC(STQ, MEMS);
		break;

	case OPC_STL_C:		// 2E - STL_C instruction
		OPC_EXEC(STL_C, MEMS);
		break;

	case OPC_STQ_C:		// 2F - STQ_C instruction
		OPC_EXEC(STQ_C, MEMS);
		break;

	case OPC_BR:		// 30 - BR instruction
		OPC_EXEC(BR, BR);
		break;

	case OPC_FBEQ:		// 31 - FBEQ instruction
		OPC_EXEC(FBEQ, FCOND);
		break;

	case OPC_FBLT:		// 32 - FBLT instruction
		OPC_EXEC(FBLT, FCOND);
		break;

	case OPC_FBLE:		// 33 - FBLE instruction
		OPC_EXEC(FBLE, FCOND);
		break;

	case OPC_BSR:		// 34 - BSR instruction
		OPC_EXEC(BSR, BR);
		break;

	case OPC_FBNE:		// 35 - FBNE instruction
		OPC_EXEC(FBNE, FCOND);
		break;

	case OPC_FBGE:		// 36 - FBGE instruction
		OPC_EXEC(FBGE, FCOND);
		break;

	case OPC_FBGT:		// 37 - FBGT instruction
		OPC_EXEC(FBGT, FCOND);
		break;

	case OPC_BLBC:		// 38 - BLBC instruction
		OPC_EXEC(BLBC, COND);
		break;

	case OPC_BEQ:		// 39 - BEQ instruction
		OPC_EXEC(BEQ, COND);
		break;

	case OPC_BLT:		// 3A - BLT instruction
		OPC_EXEC(BLT, COND);
		break;

	case OPC_BLE:		// 3B - BLE instruction
		OPC_EXEC(BLE, COND);
		break;

	case OPC_BLBS:		// 3C - BLBS instruction
		OPC_EXEC(BLBS, COND);
		break;

	case OPC_BNE:		// 3D - BNE instruction
		OPC_EXEC(BNE, COND);
		break;

	case OPC_BGE:		// 3E - BGE instruction
		OPC_EXEC(BGE, COND);
		break;

	case OPC_BGT:		// 3F - BGT instruction
		OPC_EXEC(BGT, COND);
		break;

	unimpl:
	default:
		UNKNOWN_OPCODE1;
		break;
	}

}


