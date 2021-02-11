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

	if (((func < 0x40) && (state.cm == ACC_KERNEL)) ||
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
//	case IPR_ISR:
//	case IPR_ITB_PTE:
//	case IPR_ITB_ASN:
//	case IPR_ITB_PTE_TEMP:
//	case IPR_SIRR:
//	case IPR_ASTRR:
//	case IPR_ASTER:
//		break;

	case IPR_EXC_ADDR: // Exception address register
		RAV = state.excAddr;
		break;

	case IPR_EXC_SUM:
		RAV = state.sum;
		break;

	case IPR_EXC_MASK:
		RAV = state.excMask;
		break;

	case IPR_PAL_BASE: // PAL base address register
		RAV = state.palBase;
		break;

//	case IPR_INTID:
//	case IPR_IFAULT_VA_FORM:
//	case IPR_IVPTBR:
//	case IPR_SL_RCV:
//	case IPR_ICPERR_STAT:
//	case IPR_PMCTR:
//		break;

	case IPR_ICM:
		RAV = state.cm;
		break;

	case IPR_ICSR: // Ibox Control and Status Register
		RAV = ev5.icsr;
		break;

	case IPR_IPLR:
		RAV = state.ipl;
		break;

	case IPR_MM_STAT:
		RAV = state.mmstat;
		break;

	case IPR_VA:
		RAV = state.fpcAddr;
		break;

	case IPR_VA_FORM:
		RAV = 0;
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

//#define IPR_DTB_PTE				0x203	// (R/W)
//#define IPR_DTB_PTE_TEMP		0x204	// (R)
//#define IPR_MM_STAT				0x205	// (R)
//#define IPR_VA					0x206	// (R)
//#define IPR_VA_FORM				0x207	// (R)
//#define IPR_MCSR				0x20F	// (R/W)
//#define IPR_DC_PERR_STAT		0x212	// (R/W1C)
//#define IPR_DC_TEST_CTL			0x213	// (R/W)
//#define IPR_DC_TEST_TAG			0x214	// (R/W)
//#define IPR_DC_TEST_TAG_TEMP	0x215	// (R/W)
//#define IPR_DC_MODE				0x216	// (R/W)
//#define IPR_MAF_MODE			0x217	// (R/W)

	case IPR_PALtemp0:  case IPR_PALtemp1:  case IPR_PALtemp2:  case IPR_PALtemp3:
	case IPR_PALtemp4:  case IPR_PALtemp5:  case IPR_PALtemp6:  case IPR_PALtemp7:
	case IPR_PALtemp8:  case IPR_PALtemp9:  case IPR_PALtemp10: case IPR_PALtemp11:
	case IPR_PALtemp12: case IPR_PALtemp13: case IPR_PALtemp14: case IPR_PALtemp15:
	case IPR_PALtemp16: case IPR_PALtemp17: case IPR_PALtemp18: case IPR_PALtemp19:
	case IPR_PALtemp20: case IPR_PALtemp21: case IPR_PALtemp22: case IPR_PALtemp23:
		RAV = ev5.PALtemp[fnc - IPR_PALtemp0];
		break;

	// Ignore write-only registers
//	case IPR_ITB_TAG:
//	case IPR_ITB_IA:
//	case IPR_ITB_IAP:
//	case IPR_ITB_IS:
//	case IPR_HWINT_CLR:
//	case IPR_SL_XMIT:
//	case IPR_IC_FLUSH_CTL:

//#define IPR_DTB_ASN				0x200	// (W)
//#define IPR_DTB_CM				0x201	// (W)
//#define IPR_DTB_TAG				0x202	// (W)
//#define IPR_MVPTBR				0x208	// (W)
//#define IPR_DTB_IAP				0x209	// (W)
//#define IPR_DTB_IA				0x20A	// (W)
//#define IPR_DTB_IS				0x20B	// (W)
//#define IPR_ALT_MODE			0x20C	// (W)
//#define IPR_CC					0x20D	// (W)
//#define IPR_CC_CTL				0x20E	// (W)
//#define IPR_DC_FLUSH			0x210	// (W)

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
		tbia(ACC_EXEC);
		break;

	case IPR_ITB_IAP:
		tbiap(ACC_EXEC);
		break;

	case IPR_ITB_IS:
		tbis(RBV, ACC_EXEC);
		break;

	case IPR_EXC_ADDR: // Exception Address
		state.excAddr = RBV;
		break;

	case IPR_EXC_SUM:
		state.sum = 0;
		state.excMask = 0;
		break;

	case IPR_PAL_BASE: // PAL base address register
		state.palBase = RBV & PAL_BASE_MASK;
		break;

	case IPR_ICM:
		state.cm = (RBV >> 3) & 3;
		break;

	case IPR_ICSR: // Ibox Control and Status Register
		ev5.icsr = (RBV & ICSR_RW) | ICSR_MBO;

		// Update internal registers
		state.sde  = (ev5.icsr & ICSR_SDE) ? 1 : 0;
		state.fpen = (ev5.icsr & ICSR_FPE) ? 1 : 0;
		state.ispe = (ev5.icsr & ICSR_SPE) >> 28;

		break;

	case IPR_IPLR:
		state.ipl = RBV & 0x1F;
		break;

	case IPR_IC_FLUSH_CTL: // Flush ICache
//		flushICache();
		break;

	case IPR_DTB_IA:
		tbia(ACC_READ);
		break;

	case IPR_DTB_IAP:
		tbiap(ACC_READ);
		break;

	case IPR_DTB_IS:
		tbis(RBV, ACC_READ);
		break;

	case IPR_ALT_MODE:
		state.altcm = (RBV >> 3) & 3;
		break;

	case IPR_CC:
		state.ccOffset = RBV >> 32;
		break;

	case IPR_CC_CTL:
		state.cc     = RBV & 0xFFFFFFF0;
		state.cc_ena = (RBV >> 32) & 1;
		break;

	case IPR_MCSR: // Mbox Control and Status Register
		ev5.mcsr = RBV & MCSR_RW;

		// Update internal registers
		state.mspe = (ev5.mcsr & MCSR_SP) >> 1;
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

//#define IPR_ISR					0x100	// (R)
//#define IPR_ITB_TAG				0x101	// (W)   Istream Translation Buffer Tag Register
//#define IPR_ITB_PTE				0x102	// (R/W) Instruction Translation Buffer Page Table Entry Register
//#define IPR_ITB_ASN				0x103	// (R/W) Instruction Translation Buffer Address Space Number Register
//#define IPR_ITB_PTE_TEMP		0x104	// (R)
//#define IPR_ITB_IA				0x105	// (W)
//#define IPR_ITB_IAP				0x106	// (W)
//#define IPR_ITB_IS				0x107	// (W)
//#define IPR_SIRR				0x108	// (R/W)
//#define IPR_ASTRR				0x109	// (R/W)
//#define IPR_ASTER				0x10A	// (R/W)
//#define IPR_EXC_ADDR			0x10B	// (R/W)
//#define IPR_EXC_SUM				0x10C	// (R/W0C)
//#define IPR_EXC_MASK			0x10D	// (R)
//#define IPR_PAL_BASE			0x10E	// (R/W)
//#define IPR_ICM					0x10F	// (R/W)
//#define IPR_IPLR				0x110	// (R/W)
//#define IPR_INTID				0x111	// (R)
//#define IPR_IFAULT_VA_FORM		0x112	// (R)
//#define IPR_IVPTBR				0x113	// (R/W)
//#define IPR_HWINT_CLR			0x115	// (W)
//#define IPR_SL_XMIT				0x116	// (W)
//#define IPR_SL_RCV				0x117	// (R)
//#define IPR_ICSR				0x118	// (R/W)
//#define IPR_IC_FLUSH_CTL		0x119	// (W)
//#define IPR_ICPERR_STAT			0x11A	// (R/W1C)
//#define IPR_PMCTR				0x11C	// (R/W)

//#define IPR_DTB_ASN				0x200	// (W)
//#define IPR_DTB_CM				0x201	// (W)
//#define IPR_DTB_TAG				0x202	// (W)
//#define IPR_DTB_PTE				0x203	// (R/W)
//#define IPR_DTB_PTE_TEMP		0x204	// (R)
//#define IPR_MM_STAT				0x205	// (R)
//#define IPR_VA					0x206	// (R)
//#define IPR_VA_FORM				0x207	// (R)
//#define IPR_MVPTBR				0x208	// (W)
//#define IPR_DTB_IAP				0x209	// (W)
//#define IPR_DTB_IA				0x20A	// (W)
//#define IPR_DTB_IS				0x20B	// (W)
//#define IPR_ALT_MODE			0x20C	// (W)
//#define IPR_CC					0x20D	// (W)
//#define IPR_CC_CTL				0x20E	// (W)
//#define IPR_MCSR				0x20F	// (R/W)
//#define IPR_DC_FLUSH			0x210	// (W)
//#define IPR_DC_PERR_STAT		0x212	// (R/W1C)
//#define IPR_DC_TEST_CTL			0x213	// (R/W)
//#define IPR_DC_TEST_TAG			0x214	// (R/W)
//#define IPR_DC_TEST_TAG_TEMP	0x215	// (R/W)
//#define IPR_DC_MODE				0x216	// (R/W)
//#define IPR_MAF_MODE			0x217	// (R/W)

	default:
#ifdef DEBUG
		if (dbg.checkAnyFlags(DBG_TRACE))
			dbg.log("%s(HW_MTPR): Undefined function code #%04X\n",
				getDeviceName(), fnc);
#endif /* DEBUG */
		break;
	}
}
