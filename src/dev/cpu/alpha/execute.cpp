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


void AlphaProcessor::init()
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

	// Initialize Ibox control register
	state.ictl.spce = 0;
	state.ictl.ic_en = 3;
	state.ictl.spe = 0;
	state.ictl.sde = 0;
	state.ictl.sbe = 0;
	state.ictl.bpMode = 0;
	state.ictl.hwe = 0;
	state.ictl.sl_xmit = 0;
	state.ictl.sl_rcv = 0;
	state.ictl.va48 = 0;
	state.ictl.vaForm32 = 0;
	state.ictl.single_issue_h = 0;
	state.ictl.pct0_en = 0;
	state.ictl.pct1_en = 0;
	state.ictl.call_pal_r23 = 0;
	state.ictl.mchk_en = 0;
	state.ictl.tb_mb_en = 0;
	state.ictl.bist_fail = 1;
	state.ictl.chip_id = 5;
	state.ictl.vptb = 0;
	state.ictl.sext_vptb = 0;

	state.cMode = ACC_KERNEL;

	mapProgram = getAddressSpace(AS_PROGRAM);
}

void AlphaProcessor::setPCAddress(offs_t addr)
{
	state.vpcReg = addr;
	state.ppcReg = addr;
}

void AlphaProcessor::step(Console *user)
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

void AlphaProcessor::run()
{
	// Start execution state
	pState = execRunning;

	try {
		while(pState == execRunning)
		{
			execute();
		}
	}

	catch (...)
	{
		// Flush all remaining buffers
		dbg.flushAll();
	}

	// Stop execution state
	pState = execStopped;
}

int AlphaProcessor::fetchi(uint64_t addr, uint32_t &data)
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

	data = mapProgram->read32(state.vpcReg, this);
	return 0;
}

void AlphaProcessor::execute()
{
	uint32_t opWord;
	int opCode;
	uint32_t func;
	uint64_t pAddr;

	uint64_t dbgval;
	string   dbgstr;

	if (((state.vpcReg & ~0x3ull) == 0xC000) ||
		((state.vpcReg & ~0x3ull) == 0x8000))
		dbg.setDebugFlags(DBG_TRACE);

	// Display current instruction
//	list(nullptr, state.vpcReg);

	state.cpcAddr = state.vpcReg;
//	opWord = readv32(state.vpcReg);
	fetchi(state.vpcReg, opWord);
	nextPC();

	// R31/F31 register - always zero
	state.iRegs[REG_ZERO] = 0;
	state.fRegs[REG_ZERO] = 0;

	opCode = OP_GETOP(opWord);
	switch (opCode)
	{
	case OPC_PAL:		// 00 - CALL_PAL instruction
		func = OP_GETPAL(opWord);
		OPC_EXEC2(CALL_PAL, PAL);
		break;

	case OPC_LDA:		// 08 - LDA instruction
		OPC_EXEC2(LDA, MEM);
		break;

	case OPC_LDAH:		// 09 - LDAH instruction
		OPC_EXEC2(LDAH, MEM);
		break;

	case OPC_LDBU:		// 0A - LDBU instruction
		OPC_EXEC2(LDBU, MEM);
		break;

	case OPC_LDQ_U:		// 0B - LDQ_U instruction
		OPC_EXEC2(LDQ_U, MEM);
		break;

	case OPC_LDWU:		// 0C - LDWU instruction
		OPC_EXEC2(LDWU, MEM);
		break;

	case OPC_STW:		// 0D - STW instruction
		OPC_EXEC2(STW, MEM);
		break;

	case OPC_STB:		// 0E - STB instruction
		OPC_EXEC2(STB, MEM);
		break;

	case OPC_STQ_U:		// OF - STQ_U instruction
		OPC_EXEC2(STQ_U, MEM);
		break;

	case OPC_INTA:		// 10 - Arithmetic instructions
		func = (opWord >> 5) & 0x7F;
		switch (func)
		{
		case 0x00:
			OPC_EXEC2(ADDL, RAB_RC);
			break;
		case 0x02:
			OPC_EXEC2(S4ADDL, RAB_RC);
			break;
		case 0x12:
			OPC_EXEC2(S8ADDL, RAB_RC);
			break;
//		case 0x40:
//			OPC_EXEC2(ADDL_V, RAB_RC);
//			break;

		case 0x20:
			OPC_EXEC2(ADDQ, RAB_RC);
			break;
		case 0x22:
			OPC_EXEC2(S4ADDQ, RAB_RC);
			break;
		case 0x32:
			OPC_EXEC2(S8ADDQ, RAB_RC);
			break;
//		case 0x60:
//			OPC_EXEC2(ADDQ_V, RAB_RC);
//			break;


		case 0x09:
			OPC_EXEC2(SUBL, RAB_RC);
			break;
		case 0x0B:
			OPC_EXEC2(S4SUBL, RAB_RC);
			break;
		case 0x1B:
			OPC_EXEC2(S8SUBL, RAB_RC);
			break;
//		case 0x49:
//			OPC_EXEC2(SUBL_V, RAB_RC);
//			break;

		case 0x29:
			OPC_EXEC2(SUBQ, RAB_RC);
			break;
		case 0x2B:
			OPC_EXEC2(S4SUBQ, RAB_RC);
			break;
		case 0x3B:
			OPC_EXEC2(S8SUBL, RAB_RC);
			break;
//		case 0x69:
//			OPC_EXEC2(SUBQ_V, RAB_RC);
//			break;

//		case 0x0F:
//			OPC_EXEC2(CMPBGE, RAB_RC);
//			break;

		case 0x1D:
			OPC_EXEC2(CMPULT, RAB_RC);
			break;
		case 0x2D:
			OPC_EXEC2(CMPEQ, RAB_RC);
			break;
		case 0x3D:
			OPC_EXEC2(CMPULE, RAB_RC);
			break;
		case 0x4D:
			OPC_EXEC2(CMPLT, RAB_RC);
			break;
		case 0x6D:
			OPC_EXEC2(CMPLE, RAB_RC);
			break;

		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_INTL:		// 11 - Logical instructions
		func = (opWord >> 5) & 0x7F;
		switch (func)
		{
		case 0x00:
			OPC_EXEC2(AND, RAB_RC);
			break;
		case 0x08:
			OPC_EXEC2(BIC, RAB_RC);
			break;
		case 0x14:
			OPC_EXEC2(CMOVLBS, RAB_RC);
			break;
		case 0x16:
			OPC_EXEC2(CMOVLBC, RAB_RC);
			break;
		case 0x20:
			OPC_EXEC2(BIS, RAB_RC);
			break;
		case 0x24:
			OPC_EXEC2(CMOVEQ, RAB_RC);
			break;
		case 0x26:
			OPC_EXEC2(CMOVNE, RAB_RC);
			break;
		case 0x28:
			OPC_EXEC2(ORNOT, RAB_RC);
			break;
		case 0x40:
			OPC_EXEC2(XOR, RAB_RC);
			break;
		case 0x44:
			OPC_EXEC2(CMOVLT, RAB_RC);
			break;
		case 0x46:
			OPC_EXEC2(CMOVGE, RAB_RC);
			break;
		case 0x48:
			OPC_EXEC2(EQV, RAB_RC);
			break;
//		case 0x61:
//			OPC_EXEC2(AMASK, RAB_RC);
//			break;
		case 0x64:
			OPC_EXEC2(CMOVLE, RAB_RC);
			break;
		case 0x66:
			OPC_EXEC2(CMOVGT, RAB_RC);
			break;
//		case 0x6C:
//			OPC_EXEC(IMPLVER, X_R3);
//			break;
		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_INTS:		// 12 - Shift instructions
		func = (opWord >> 5) & 0x7F;
		switch (func)
		{
	    case 0x02:
	      OPC_EXEC2(MSKBL, RAB_RC);
	      break;
	    case 0x06:
	      OPC_EXEC2(EXTBL, RAB_RC);
	      break;
	    case 0x0B:
	      OPC_EXEC2(INSBL, RAB_RC);
	      break;
	    case 0x12:
	      OPC_EXEC2(MSKWL, RAB_RC);
	      break;
	    case 0x16:
	      OPC_EXEC2(EXTWL, RAB_RC);
	      break;
	    case 0x1B:
	      OPC_EXEC2(INSWL, RAB_RC);
	      break;
	    case 0x22:
	      OPC_EXEC2(MSKLL, RAB_RC);
	      break;
	    case 0x26:
	      OPC_EXEC2(EXTLL, RAB_RC);
	      break;
	    case 0x2b:
	      OPC_EXEC2(INSLL, RAB_RC);
	      break;
	    case 0x30:
	      OPC_EXEC2(ZAP, RAB_RC);
	      break;
	    case 0x31:
	      OPC_EXEC2(ZAPNOT, RAB_RC);
	      break;
	    case 0x32:
	      OPC_EXEC2(MSKQL, RAB_RC);
	      break;
	    case 0x34:
	      OPC_EXEC2(SRL, RAB_RC);
	      break;
	    case 0x36:
	      OPC_EXEC2(EXTQL, RAB_RC);
	      break;
	    case 0x39:
	      OPC_EXEC2(SLL, RAB_RC);
	      break;
	    case 0x3B:
	      OPC_EXEC2(INSQL, RAB_RC);
	      break;
	    case 0x3C:
	      OPC_EXEC2(SRA, RAB_RC);
	      break;
	    case 0x52:
	      OPC_EXEC2(MSKWH, RAB_RC);
	      break;
	    case 0x57:
	      OPC_EXEC2(INSWH, RAB_RC);
	      break;
	    case 0x5A:
	      OPC_EXEC2(EXTWH, RAB_RC);
	      break;
	    case 0x62:
	      OPC_EXEC2(MSKLH, RAB_RC);
	      break;
	    case 0x67:
	      OPC_EXEC2(INSLH, RAB_RC);
	      break;
	    case 0x6A:
	      OPC_EXEC2(EXTLH, RAB_RC);
	      break;
	    case 0x72:
	      OPC_EXEC2(MSKQH, RAB_RC);
	      break;
	    case 0x77:
	      OPC_EXEC2(INSQH, RAB_RC);
	      break;
	    case 0x7a:
	      OPC_EXEC2(EXTQH, RAB_RC);
	      break;
		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_INTM:		// 13 - Multiply instructions
		func = (opWord >> 5) & 0x7F;
		switch (func)
		{
		case 0x00:
			OPC_EXEC2(MULL, RAB_RC);
			break;
		case 0x20:
			OPC_EXEC2(MULQ, RAB_RC);
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
	case OPC_FLTV:		// 15 - Floating VAX instructions
	case OPC_FLTI:		// 16 - Floating IEEE instructions
	case OPC_FLTL:		// 17 - FLTL instruction
		goto unimpl;

	case OPC_MISC:		// 18 - Miscellaneous instructions
		func = opWord & 0xFFFF;
		switch (func)
		{
		case 0x0000: // 0000 - TRAPB
			OPC_EXEC2(TRAPB, NOP);
			break;
		case 0x0400: // EXCB
			OPC_EXEC2(EXCB, NOP);
			break;
		case 0x4000: // MB
			OPC_EXEC2(MB, NOP);
			break;
		case 0x4400: // WMB
			OPC_EXEC2(WMB, NOP);
			break;
		case 0x8000: // FETCH
			OPC_EXEC2(FETCH, NOP);
			break;
		case 0xA000: // FETCH_M
			OPC_EXEC2(FETCH_M, NOP);
			break;
//		case 0xC000: // RPCC
//			OPC_EXEC(RPCC, X_R1);
//			break;
//		case 0xE000: // RC
//			OPC_EXEC(RC, X_R1);
//			break;
		case 0xE800: // ECB
			OPC_EXEC2(ECB, NOP);
			break;
//		case 0xF000: // RS
//			OPC_EXEC(RS, X_R1);
//			break;
		case 0xF800: // WH64
			OPC_EXEC2(WH64, NOP);
			break;
		case 0xFC00: // WH64EN
			OPC_EXEC2(WH64EN, NOP);
			break;
		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_HW_MFPR:	// 19 - HW_MFPR instruction
		func = (opWord >> 8) & 0xFF;
//		OPC_EXEC2(HW_MFPR, MFPR);
		OPC_FUNC(HW_MFPR, hw_mfpr, MFPR);
		break;

	case OPC_JSR:		// 1A - JSR instruction
		OPC_EXEC2(JMP, JMP);
		break;

	case OPC_HW_LD:		// 1B - HW_LD instruction
		func = (opWord >> 12) & 0xF;
		if (func & 1) {
			OPC_EXEC2(HW_LDQ, HW_LD);
		} else {
			OPC_EXEC2(HW_LDL, HW_LD);
		}
		break;

	case OPC_FPTI:		// 1C - Floating/Integer instructions
		goto unimpl;

	case OPC_HW_MTPR:	// 1D - HW_MTPR instruction
		func = (opWord >> 8) & 0xFF;
//		OPC_EXEC2(HW_MTPR, MTPR);
		OPC_FUNC(HW_MTPR, hw_mtpr, MTPR);
		break;

	case OPC_RET:		// 1E - RET instruction
		OPC_EXEC2(HW_REI, RET);
		break;

	case OPC_HW_ST:		// 1F - HW_ST instruction
		func = (opWord >> 12) & 0xF;
		if (func & 1) {
			OPC_EXEC2(HW_STQ, HW_ST);
		} else {
			OPC_EXEC2(HW_STL, HW_ST);
		}
		break;

	case OPC_LDF:		// 20 - LDF instruction
	case OPC_LDG:		// 21 - LDG instruction
	case OPC_LDS:		// 22 - LDS instruction
	case OPC_LDT:		// 23 - LDT instruction
	case OPC_STF:		// 24 - STF instruction
	case OPC_STG:		// 25 - STG instruction
	case OPC_STS:		// 26 - STS instruction
	case OPC_STT:		// 27 - STT instruction
		goto unimpl;

	case OPC_LDL:		// 28 - LDL instruction
		OPC_EXEC2(LDL, MEM);
		break;

	case OPC_LDQ:		// 29 - LDQ instruction
		OPC_EXEC2(LDQ, MEM);
		break;

	case OPC_LDL_L:		// 2A - LDL_L instruction
		OPC_EXEC2(LDL_L, MEM);
		break;

	case OPC_LDQ_L:		// 2B - LDQ_L instruction
		OPC_EXEC2(LDQ_L, MEM);
		break;

	case OPC_STL:		// 2C - STL instruction
		OPC_EXEC2(STL, MEM);
		break;

	case OPC_STQ:		// 2D - STQ instruction
		OPC_EXEC2(STQ, MEM);
		break;

	case OPC_STL_C:		// 2E - STL_C instruction
		OPC_EXEC2(STL_C, MEM);
		break;

	case OPC_STQ_C:		// 2F - STQ_C instruction
		OPC_EXEC2(STQ_C, MEM);
		break;

	case OPC_BR:		// 30 - BR instruction
		OPC_EXEC2(BR, BR);
		break;

	case OPC_FBEQ:		// 31 - FBEQ instruction
	case OPC_FBLT:		// 32 - FBLT instruction
	case OPC_FBLE:		// 33 - FBLE instruction
		goto unimpl;

	case OPC_BSR:		// 34 - BSR instruction
		OPC_EXEC2(BSR, BR);
		break;

	case OPC_FBNE:		// 35 - FBNE instruction
	case OPC_FBGE:		// 36 - FBGE instruction
	case OPC_FBGT:		// 37 - FBGT instruction
		goto unimpl;

	case OPC_BLBC:		// 38 - BLBC instruction
		OPC_EXEC2(BLBC, COND);
		break;

	case OPC_BEQ:		// 39 - BEQ instruction
		OPC_EXEC2(BEQ, COND);
		break;

	case OPC_BLT:		// 3A - BLT instruction
		OPC_EXEC2(BLT, COND);
		break;

	case OPC_BLE:		// 3B - BLE instruction
		OPC_EXEC2(BLE, COND);
		break;

	case OPC_BLBS:		// 3C - BLBS instruction
		OPC_EXEC2(BLBS, COND);
		break;

	case OPC_BNE:		// 3D - BNE instruction
		OPC_EXEC2(BNE, COND);
		break;

	case OPC_BGE:		// 3E - BGE instruction
		OPC_EXEC2(BGE, COND);
		break;

	case OPC_BGT:		// 3F - BGT instruction
		OPC_EXEC2(BGT, COND);
		break;

	unimpl:
	default:
		UNKNOWN_OPCODE1;
		break;
	}

}


