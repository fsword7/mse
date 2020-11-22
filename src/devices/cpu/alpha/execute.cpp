/*
 * execute.cpp - Alpha CPU Processor package - execute routines
 *
 *  Created on: Nov 21, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/sysconfig.h"
#include "emu/device.h"
#include "devices/cpu/alpha/axp.h"
#include "devices/cpu/alpha/opcode.h"
#include "devices/cpu/alpha/debug.h"

// instructions function defines
#include "devices/cpu/alpha/axp_arith.h"
#include "devices/cpu/alpha/axp_branch.h"
#include "devices/cpu/alpha/axp_mem.h"


void AlphaProcessor::init()
{

	// Clear all regular/shadow registers
	for (int idx = 0; idx < AXP_NIREGS*2; idx++)
		state.iRegs[idx] = 0;
	for (int idx = 0; idx < AXP_NFREGS*2; idx++)
		state.fRegs[idx] = 0;

}

void AlphaProcessor::execute()
{
	uint32_t inst;
	int opCode;
	uint32_t func;

	// R31/F31 register - always zero
	state.iRegs[AXP_NIREGS-1] = 0;
	state.fRegs[AXP_NFREGS-1] = 0;

	opCode = OP_GETCD(inst);
	switch (opCode)
	{
	case OPC_PAL:		// 00 - CALL_PAL instruction
		func = OP_GETPAL(inst);
		goto unimpl;

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
		OPC_EXEC(STW, MEM);
		break;

	case OPC_STB:		// 0E - STB instruction
		OPC_EXEC(STB, MEM);
		break;

	case OPC_STQ_U:		// OF - STQ_U instruction
		OPC_EXEC(STQ_U, MEM);
		break;

	case OPC_INTA:		// 10 - Arithmetic instructions
		func = (inst >> 5) & 0x7F;
		switch (func)
		{
		case 0x00:
			OPC_EXEC(ADDL, R12_R3);
			break;
		case 0x02:
			OPC_EXEC(S4ADDL, R12_R3);
			break;
		case 0x12:
			OPC_EXEC(S8ADDL, R12_R3);
			break;
//		case 0x40:
//			OPC_EXEC(ADDL_V, RF12_R3);
//			break;

		case 0x20:
			OPC_EXEC(ADDQ, R12_R3);
			break;
		case 0x22:
			OPC_EXEC(S4ADDQ, R12_R3);
			break;
		case 0x32:
			OPC_EXEC(S8ADDQ, R12_R3);
			break;
//		case 0x60:
//			OPC_EXEC(ADDQ_V, R12_R3);
//			break;


		case 0x09:
			OPC_EXEC(SUBL, R12_R3);
			break;
		case 0x0B:
			OPC_EXEC(S4SUBL, R12_R3);
			break;
		case 0x1B:
			OPC_EXEC(S8SUBL, R12_R3);
			break;
//		case 0x49:
//			OPC_EXEC(SUBL_V, R12_R3);
//			break;

		case 0x29:
			OPC_EXEC(SUBQ, R12_R3);
			break;
		case 0x2B:
			OPC_EXEC(S4SUBQ, R12_R3);
			break;
		case 0x3B:
			OPC_EXEC(S8SUBL, R12_R3);
			break;
//		case 0x69:
//			OPC_EXEC(SUBQ_V, R12_R3);
//			break;

//		case 0x0F:
//			OPC_EXEC(CMPBGE, R12_R3);
//			break;

		case 0x1D:
			OPC_EXEC(CMPULT, R12_R3);
			break;
		case 0x2D:
			OPC_EXEC(CMPEQ, R12_R3);
			break;
		case 0x3D:
			OPC_EXEC(CMPULE, R12_R3);
			break;
		case 0x4D:
			OPC_EXEC(CMPLT, R12_R3);
			break;
		case 0x6D:
			OPC_EXEC(CMPLE, R12_R3);
			break;

		default:
			UNKNOWN_OPCODE2;
			break;
		}
		break;

	case OPC_INTL:		// 11 - Logical instructions
	case OPC_INTS:		// 12 - Shift instructions
		goto unimpl;

	case OPC_INTM:		// 13 - Multiply instructions
		func = (inst >> 5) & 0x7F;
		switch (func)
		{
		case 0x00:
			OPC_EXEC(MULL, R12_R3);
			break;
		case 0x20:
			OPC_EXEC(MULQ, R12_R3);
			break;
//		case 0x40:
//			OP_EXEC(MULL_V, R12_R3);
//			break;
//		case 0x60:
//			OP_EXEC(MULQ_V, R12_R3);
//			break;
//		case 0x30:
//			OP_EXEC(UMULH, R12_R3);
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
	case OPC_MISC:		// 18 - Miscellaneous instructions
	case OPC_HW_MFPR:	// 19 - HW_MFPR instruction
		goto unimpl;

	case OPC_JSR:		// 1A - JSR instruction
		OPC_EXEC(JSR, DISP);
		break;

	case OPC_HW_LD:		// 1B - HW_LD instruction
	case OPC_FPTI:		// 1C - Floating/Integer instructions
	case OPC_HW_MTPR:	// 1D - HW_MTPR instruction
		break;

	case OPC_RET:		// 1E - RET instruction
		OPC_EXEC(RET, DISP);
		break;

	case OPC_HW_ST:		// 1F - HW_ST instruction
		goto unimpl;

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
		OPC_EXEC(STL, MEM);
		break;

	case OPC_STQ:		// 2D - STQ instruction
		OPC_EXEC(STQ, MEM);
		break;

	case OPC_STL_C:		// 2E - STL_C instruction
		OPC_EXEC(STL_C, MEM);
		break;

	case OPC_STQ_C:		// 2F - STQ_C instruction
		OPC_EXEC(STQ_C, MEM);
		break;

	case OPC_BR:		// 30 - BR instruction
		OPC_EXEC(BR, DISP);
		break;

	case OPC_FBEQ:		// 31 - FBEQ instruction
	case OPC_FBLT:		// 32 - FBLT instruction
	case OPC_FBLE:		// 33 - FBLE instruction
		goto unimpl;

	case OPC_BSR:		// 34 - BSR instruction
		OPC_EXEC(BSR, DISP);
		break;

	case OPC_FBNE:		// 35 - FBNE instruction
	case OPC_FBGE:		// 36 - FBGE instruction
	case OPC_FBGT:		// 37 - FBGT instruction
		goto unimpl;

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


