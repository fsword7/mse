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
		break;

	case OPC_LDA:		// 08 - LDA instruction
	case OPC_LDAH:		// 09 - LDAH instruction
	case OPC_LDBU:		// 0A - LDBU instruction
	case OPC_LDQ_U:		// 0B - LDQ_U instruction
	case OPC_LDWU:		// 0C - LDWU instruction
	case OPC_STW:		// 0D - STW instruction
	case OPC_STB:		// 0E - STB instruction
	case OPC_STQ_U:		// OF - STQ_U instruction
	case OPC_INTA:		// 10 - Arithmetic instructions
	case OPC_INTL:		// 11 - Logical instructions
	case OPC_INTS:		// 12 - Shift instructions
	case OPC_INTM:		// 13 - Multiply instructions
	case OPC_ITFP:		// 14 - Integer/Floating instructions
	case OPC_FLTV:		// 15 - Floating VAX instructions
	case OPC_FLTI:		// 16 - Floating IEEE instructions
	case OPC_FLTL:		// 17 - FLTL instruction
	case OPC_MISC:		// 18 - Miscellaneous instructions
	case OPC_HW_MFPR:	// 19 - HW_MFPR instruction
	case OPC_JSR:		// 1A - JSR instruction
	case OPC_HW_LD:		// 1B - HW_LD instruction
	case OPC_FPTI:		// 1C - Floating/Integer instructions
	case OPC_HW_MTPR:	// 1D - HW_MTPR instruction
	case OPC_RET:		// 1E - RET instruction
	case OPC_HW_ST:		// 1F - HW_ST instruction
	case OPC_LDF:		// 20 - LDF instruction
	case OPC_LDG:		// 21 - LDG instruction
	case OPC_LDS:		// 22 - LDS instruction
	case OPC_LDT:		// 23 - LDT instruction
	case OPC_STF:		// 24 - STF instruction
	case OPC_STG:		// 25 - STG instruction
	case OPC_STS:		// 26 - STS instruction
	case OPC_STT:		// 27 - STT instruction
	case OPC_LDL:		// 28 - LDL instruction
	case OPC_LDQ:		// 29 - LDQ instruction
	case OPC_LDL_L:		// 2A - LDL_L instruction
	case OPC_LDQ_L:		// 2B - LDQ_L instruction
	case OPC_STL:		// 2C - STL instruction
	case OPC_STQ:		// 2D - STQ instruction
	case OPC_STL_C:		// 2E - STL_C instruction
	case OPC_STQ_C:		// 2F - STQ_C instruction
	case OPC_BR:		// 30 - BR instruction
	case OPC_FBEQ:		// 31 - FBEQ instruction
	case OPC_FBLT:		// 32 - FBLT instruction
	case OPC_FBLE:		// 33 - FBLE instruction
	case OPC_BSR:		// 34 - BSR instruction
	case OPC_FBNE:		// 35 - FBNE instruction
	case OPC_FBGE:		// 36 - FBGE instruction
	case OPC_FBGT:		// 37 - FBGT instruction
	case OPC_BLBC:		// 38 - BLBC instruction
	case OPC_BEQ:		// 39 - BEQ instruction
	case OPC_BLT:		// 3A - BLT instruction
	case OPC_BLE:		// 3B - BLE instruction
	case OPC_BLBS:		// 3C - BLBS instruction
	case OPC_BNE:		// 3D - BNE instruction
	case OPC_BGE:		// 3E - BGE instruction
	case OPC_BGT:		// 3F - BGT instruction
		break;

	default:
		break;
	}

}


