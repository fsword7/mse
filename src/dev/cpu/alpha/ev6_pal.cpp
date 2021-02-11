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

	if ((fnc & 0xC0) == 0x40)
	{
		// PCTX0 register (40 - 5F)
		RAV = (uint64_t(state.asn) << 39)  | (uint64_t(state.astrr) << 9) |
			  (uint64_t(state.aster) << 5) | (state.fpen ? 0x4 : 0) |
			  (state.ppcen ? 0x2 : 0);
	}
	else
	{
		switch (fnc)
		{
//		case IPR_PMPC:

		case IPR_EXC_ADDR: // Exception address register
			RAV = state.excAddr;
			break;

		case IPR_IVA_FORM:
			RAV = 0; // (get_va_form(state.excAddr, true);
			break;

//		case IPR_IER_CM:
		case IPR_CM:
		case IPR_IER:
		case IPR_IER_CM:
			RAV = (uint64_t(state.eien) << 33) | (uint64_t(state.slen) << 32) |
			      (uint64_t(state.cren) << 31) | (uint64_t(state.pcen) << 29) |
				  (uint64_t(state.sien) << 13) | (uint64_t(state.asten) << 13) |
				  (uint64_t(state.cm) << 3);
			break;

		case IPR_SIRR:
			RAV = (uint64_t(state.sir) << 13);
			break;

		case IPR_ISUM:
			break;

		case IPR_EXC_SUM:
			RAV = state.sum;
			break;

		case IPR_PAL_BASE: // PAL base address register
			RAV = state.palBase;
			break;

		case IPR_I_CTL: // Ibox Control register
			READ_ICTL(RAV);
			break;

		case IPR_PCTR_CTL:
			RAV = state.pctr;
			break;

		case IPR_I_STAT:
			RAV = state.istat;
			break;

		case IPR_MM_STAT:
			RAV = state.mmstat;
			break;

		case IPR_DC_STAT:
			RAV = state.dcstat;
			break;

		case IPR_C_DATA:
			RAV = 0;
			break;

		case IPR_CC:
			RAV = ((uint64_t(state.ccOffset) << 32) | (state.cc & 0xFFFFFFFF));
			break;

		case IPR_VA:
			RAV = 0; // state.fvAddr;
			break;

		case IPR_VA_FORM:
			RAV = 0; // get_va_form(state.fvAddr, false);
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
}

// HW_MTPR instruction
void dec21264_cpuDevice::hw_mtpr(uint32_t opWord)
{
	int fnc = (opWord >> 8) & 0xFF;
	uint64_t rbv;

	if ((fnc & 0xC0) == 0x40)
	{
		// PXCT register
		if (fnc & 1)
			state.asn = (RBV >> 39) & 0xFF;
		if (fnc & 2) {
			state.aster = (RBV >> 5) & 0xF;
			state.irq = true;
		}
		if (fnc & 4) {
			state.astrr = (RBV >> 9) & 0xF;
			state.irq = true;
		}
		if (fnc & 8)
			state.ppcen = (RBV >> 1) & 1;
		if (fnc & 16)
			state.fpen = (RBV >> 2) & 1;
	}
	else
	{
	switch (fnc)
	{
		case IPR_ITB_TAG:
			state.tbvAddr = RBV;
			break;

		case IPR_ITB_PTE:
			addITBEntry(state.tbvAddr, RBV);
			break;

		case IPR_ITB_IAP:
			tbiap(ACC_EXEC);
			break;

		case IPR_ITB_IA:
			tbia(ACC_EXEC);
			break;

		case IPR_ITB_IS:
			tbis(RBV, ACC_EXEC);
			break;

		case IPR_CM:
			state.cm = (RBV >> 3) & 3;
			break;

		case IPR_IER_CM:
			state.cm = (RBV >> 3) & 3;
		case IPR_IER:
			state.asten = (RBV >> 13) & 1;
			state.sien  = (RBV >> 13) & 0xFFFE;
			state.pcen  = (RBV >> 29) & 3;
			state.cren  = (RBV >> 31) & 1;
			state.slen  = (RBV >> 32) & 1;
			state.eien  = (RBV >> 33) & 0xF;
			state.irq   = true;
			break;

		case IPR_SIRR:
			state.sir = (RBV >> 13) & 0xFFFE;
			state.irq = true;
			break;

		case IPR_HW_INT_CLR:
			state.pcr &= ~((RBV >> 29) & 0x03ULL);
			state.crr &= ~((RBV >> 31) & 0x01ULL);
			state.sir &= ~((RBV >> 32) & 0x01ULL);
			break;

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

		case IPR_PCTR_CTL:
			state.pctr = RBV & 0xFFFFFFFFFFFFFFDFULL;
			break;

		case IPR_CLR_MAP:
		case IPR_SLEEP:
		case IPR_MM_STAT:
		case IPR_C_DATA:
		case IPR_C_SHFT:
//		case IPR_M_FIX:
			break;

		case IPR_I_STAT:
			state.istat &= ~RBV; // W1C
			break;

		case IPR_DTB_TAG0:
			state.tbvAddr = RBV;
			break;

		case IPR_DTB_PTE0:
			addDTBEntry(state.tbvAddr, RBV);
			break;

		case IPR_DTB_IS0:
			tbis(RBV, ACC_READ);
			break;

		case IPR_DTB_ASN0:
			state.asn0 = RBV >> 56;
			break;

		case IPR_DTB_ALTMODE:
			state.altcm = RBV & 3;
			break;

		case IPR_M_CTL:
			break;

		case IPR_DC_CTL:
			break;

		case IPR_DC_STAT:
			state.dcstat &= ~RBV;
			break;

		case IPR_DTB_TAG1:
			state.tbvAddr = RBV;
			break;

		case IPR_DTB_PTE1:
			addDTBEntry(state.tbvAddr, RBV);
			break;

		case IPR_DTB_IAP:
			tbiap(ACC_READ);
			break;

		case IPR_DTB_IA:
			tbia(ACC_READ);
			break;

		case IPR_DTB_IS1:
			tbis(RBV, ACC_READ);
			break;

		case IPR_DTB_ASN1:
			state.asn1 = RBV >> 56;
			break;

		case IPR_CC:
			state.ccOffset = RBV >> 32;
			break;

		case IPR_CC_CTL:
			state.cc_ena = (RBV >> 32) & 1;
			state.cc = RBV & 0xFFFFFFF0;
			break;

		case IPR_VA_CTL:
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
}
