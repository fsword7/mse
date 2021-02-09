/*
 * ev6arch.cpp - EV6 architecture (21164 series)
 *
 *  Created on: Feb 5, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/alpha/axp.h"
#include "dev/cpu/alpha/ev6.h"
#include "dev/cpu/alpha/ev6_ipr.h"

// CALL_PALL instruction
void dec21264_cpuDevice::call_pal(uint32_t opWord)
{
	uint32_t func = OP_GETPAL(opWord);
	uint64_t pAddr;

	if (((func < 0x40) && (state.cMode == ACC_KERNEL)) ||
       ((func >= 0x80) && (func < 0xC0)))
	{
		if (func == 0)
		{
			dbg.logf(LOG_CONSOLE, "%s: *** HALT instruction at %X\n",
				getDeviceName(), state.fpcAddr);
			dbg.flushAll();
			pState = execStopping;
			return;
		}

		// Save current PC address for HW_REI instruction
		state.iRegs[32 + 23] = state.pcAddr;
		pAddr = state.palBase | (1u << 13) | 1 |
			((func & 0x80) << 5) | ((func & 0x3F) << 6);
		setPC(pAddr);
	}
//	else {
//		UNKNOWN_OPCODE2;
//	}
}

// HW_RET instruction
void dec21264_cpuDevice::hw_ret(uint32_t opWord)
{
	setPC(state.iRegs[RB]);
}

// HW_LD Instruction
void dec21264_cpuDevice::hw_ld(uint32_t opWord)
{
	//#define DOPC_HW_LDL                                        \
	//	switch (func)                                          \
	//	{                                                      \
	//		case 0: /* longword physical */                    \
	//			pAddr = RBV + DISP12;                          \
	//			RAV = mapProgram->read32(pAddr);               \
	//			break;                                         \
	//		case 2: /* longword physical conditional */        \
	//		case 4: /* longword virtual */                     \
	//		case 12: /* longword virtual alt */                \
	//		default:                                           \
	//			UNKNOWN_OPCODE2;                               \
	//			break;                                         \
	//	}
	//
	//#define DOPC_HW_LDQ                                        \
	//	switch (func)                                          \
	//	{                                                      \
	//		case 1: /* quadword physical */                    \
	//			pAddr = RBV + DISP12;                          \
	//			RAV = mapProgram->read64(pAddr);               \
	//			break;                                         \
	//		case 3: /* quadword physical conditional */        \
	//		case 5: /* quadword virtual */                     \
	//		case 13: /* quadword virtual alt */                \
	//		default:                                           \
	//			UNKNOWN_OPCODE2;                               \
	//			break;                                         \
	//	}

}

// HW_ST instruction
void dec21264_cpuDevice::hw_st(uint32_t opWord)
{
	//#define DOPC_HW_STL                                        \
	//	switch (func)                                          \
	//	{                                                      \
	//		case 0: /* longword physical */                    \
	//			pAddr = RBV + DISP12;                          \
	//			mapProgram->write32(pAddr, RAV);               \
	//			break;                                         \
	//		case 2: /* longword physical conditional */        \
	//		case 4: /* longword virtual */                     \
	//		case 12: /* longword virtual alt */                \
	//		default:                                           \
	//			UNKNOWN_OPCODE2;                               \
	//			break;                                         \
	//	}
	//
	//
	//#define DOPC_HW_STQ                                        \
	//	switch (func)                                          \
	//	{                                                      \
	//		case 1: /* quadword physical */                    \
	//			pAddr = RBV + DISP12;                          \
	//			mapProgram->write64(pAddr, RAV);               \
	//			break;                                         \
	//		case 3: /* quadword physical conditional */        \
	//		case 5: /* quadword virtual */                     \
	//		case 13: /* quadword virtual alt */                \
	//		default:                                           \
	//			UNKNOWN_OPCODE2;                               \
	//			break;                                         \
	//	}

}

// HW_MFPR instruction
void dec21264_cpuDevice::hw_mfpr(uint32_t opWord)
{
	int fnc = (opWord >> 8) & 0xFF;

	switch (fnc)
	{
	case IPR_EXC_ADDR: // Exception address register
		RAV = state.excAddr;
		break;

	case IPR_PAL_BASE: // PAL base address register
		RAV = state.palBase;
		break;

	case IPR_I_CTL: // Ibox Control register
		READ_ICTL(RAV);
		break;

	default:
#ifdef DEBUG
		if (dbg.checkAnyFlags(DBG_TRACE))
			dbg.log("%s(HW_MTPR): Undefined function code #%02X\n",
				getDeviceName(), fnc);
#endif /* DEBUG */
		RAV = 0;
		break;
	}
}

// HW_MTPR instruction
void dec21264_cpuDevice::hw_mtpr(uint32_t opWord)
{
	int fnc = (opWord >> 8) & 0xFF;
	uint64_t rbv;

	switch (fnc)
	{
	case IPR_PAL_BASE: // PAL base address register
		state.palBase = RBV & PAL_BASE_MASK;
		break;

	case IPR_I_CTL: // Ibox control register
		WRITE_ICTL(RBV);

		// Update internal registers
		state.sde = (ev6.ictl.sde & 2) ? 1 : 0;
		break;

	case IPR_IC_FLUSH_ASM: // IC_FLUSH_ASM
//		flushICacheASM();
		break;

	case IPR_IC_FLUSH: // IC_FLUSH
//		flushICache();
		break;

	default:
#ifdef DEBUG
		if (dbg.checkAnyFlags(DBG_TRACE))
			dbg.log("%s(HW_MTPR): Undefined function code #%04X\n",
				getDeviceName(), fnc);
#endif /* DEBUG */
		break;
	}
}
