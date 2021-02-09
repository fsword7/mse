/*
 * ev5_pal.cpp - 21164 series - EV5 PAL hardware instructions
 *
 *  Created on: Feb 6, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/alpha/ev5.h"

// CALL_PALL instruction
void dec21164_cpuDevice::call_pal(uint32_t opWord)
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
		state.excAddr = state.pcAddr;
		pAddr = state.palBase | (1u << 13) | 1 |
			((func & 0x80) << 5) | ((func & 0x3F) << 6);
		setPC(pAddr);
	}
//	else {
//		UNKNOWN_OPCODE2;
//	}

}

// HW_REI instruction
void dec21164_cpuDevice::hw_ret(uint32_t opWord)
{
	setPC(state.excAddr);
}

// HW_LD Instruction
void dec21164_cpuDevice::hw_ld(uint32_t opWord)
{
//#define DOPC_HW_LDL		RAV = mapProgram->read32(RBV + DISP12);
//#define DOPC_HW_LDQ		RAV = mapProgram->read64(RBV + DISP12);
}

// HW_ST instruction
void dec21164_cpuDevice::hw_st(uint32_t opWord)
{
//#define DOPC_HW_STL		mapProgram->write32(RBV + DISP12, RAV);
//#define DOPC_HW_STQ		mapProgram->write64(RBV + DISP12, RAV);
}

// HW_MTPR instruction
void dec21164_cpuDevice::hw_mfpr(uint32_t opWord)
{
	int fnc = opWord & 0xFFFF;

	switch (fnc)
	{
	case IPR_EXC_ADDR: // Exception address register
		RAV = state.excAddr;
		break;

	case IPR_PAL_BASE: // PAL base address register
		RAV = state.palBase;
		break;

	case IPR_ICSR: // Ibox Control and Status Register
		RAV = ev5.icsr;
		break;

	case IPR_MCSR: // Mbox Control and Status Register
		RAV = ev5.mcsr;
		break;

	case IPR_DC_TEST_CTL: // Dcache Test Register
		RAV = ev5.dc_test;
		break;

	case IPR_DC_MODE: // Dcache Mode Register
		RAV = ev5.dc_mode;
		break;

	case IPR_MAF_MODE: // Missing Address
		RAV = ev5.maf_mode;
		break;

	case IPR_PALtemp0:  case IPR_PALtemp1:  case IPR_PALtemp2:  case IPR_PALtemp3:
	case IPR_PALtemp4:  case IPR_PALtemp5:  case IPR_PALtemp6:  case IPR_PALtemp7:
	case IPR_PALtemp8:  case IPR_PALtemp9:  case IPR_PALtemp10: case IPR_PALtemp11:
	case IPR_PALtemp12: case IPR_PALtemp13: case IPR_PALtemp14: case IPR_PALtemp15:
	case IPR_PALtemp16: case IPR_PALtemp17: case IPR_PALtemp18: case IPR_PALtemp19:
	case IPR_PALtemp20: case IPR_PALtemp21: case IPR_PALtemp22: case IPR_PALtemp23:
		RAV = ev5.PALtemp[fnc - IPR_PALtemp0];
		break;

	default:
#ifdef DEBUG
		if (dbg.checkAnyFlags(DBG_TRACE))
			dbg.log("%s(HW_MTPR): Undefined function code #%04X\n",
				getDeviceName(), fnc);
#endif /* DEBUG */
		RAV = 0;
		break;
	}

}

// HW_MFPR instruction
void dec21164_cpuDevice::hw_mtpr(uint32_t opWord)
{
	int fnc = opWord & 0xFFFF;
	uint64_t rbv;

	switch (fnc)
	{
	case IPR_ITB_IA:
	case IPR_ITB_IAP:
	case IPR_ITB_IS:
		// Clear all translation buffers
		break;

	case IPR_PAL_BASE: // PAL base address register
		state.palBase = RBV & PAL_BASE_MASK;
		break;

	case IPR_ICSR: // Ibox Control and Status Register
		ev5.icsr = (RBV & ICSR_RW) | ICSR_MBO;

		// Update internal registers
		state.sde = (ev5.icsr & ICSR_SDE) ? 1 : 0;

		break;

	case IPR_IC_FLUSH_CTL: // Flush ICache
//		flushICache();
		break;

	case IPR_DTB_IA:
	case IPR_DTB_IAP:
	case IPR_DTB_IS:
		// Clear all translation buffers
		break;

	case IPR_MCSR: // Mbox Control and Status Register
		ev5.mcsr = RBV & MCSR_RW;
		break;

	case IPR_DC_FLUSH:
//		flushDCache();
		break;

	case IPR_DC_TEST_CTL: // DCache Test Tag
		ev5.dc_test = RBV & DC_TEST_RW;
		break;

	case IPR_DC_MODE:
		ev5.dc_mode = RBV & DC_MODE_RW;
		break;

	case IPR_MAF_MODE:
		ev5.maf_mode = RBV;
		break;

	case IPR_PALtemp0:  case IPR_PALtemp1:  case IPR_PALtemp2:  case IPR_PALtemp3:
	case IPR_PALtemp4:  case IPR_PALtemp5:  case IPR_PALtemp6:  case IPR_PALtemp7:
	case IPR_PALtemp8:  case IPR_PALtemp9:  case IPR_PALtemp10: case IPR_PALtemp11:
	case IPR_PALtemp12: case IPR_PALtemp13: case IPR_PALtemp14: case IPR_PALtemp15:
	case IPR_PALtemp16: case IPR_PALtemp17: case IPR_PALtemp18: case IPR_PALtemp19:
	case IPR_PALtemp20: case IPR_PALtemp21: case IPR_PALtemp22: case IPR_PALtemp23:
		ev5.PALtemp[fnc - IPR_PALtemp0] = RBV;
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
