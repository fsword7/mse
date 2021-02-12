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

	if (((func < 0x40) && (state.icm == ACC_KERNEL)) ||
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
//	case IPR_ITB_PTE:
//	case IPR_ITB_ASN:
//	case IPR_ITB_PTE_TEMP:
//		break;

	case IPR_ISR:
		RAV = (state.sir << 3);
		break;

	case IPR_SIRR:
		RAV = state.sir << 3;
		break;

	case IPR_ASTRR:
		RAV = state.astrr;
		break;

	case IPR_ASTER:
		RAV = state.aster;
		break;

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

	case IPR_ICM:
		RAV = state.icm;
		break;

	case IPR_SL_RCV:
		// TODO serial line receiver
		break;

	case IPR_ICSR: // Ibox Control and Status Register
		RAV = ev5.icsr;
		break;

	case IPR_IPLR:
		RAV = state.iplr;
		break;

	case IPR_INTID:
		RAV = state.intid;
		break;

	case IPR_IFAULT_VA_FORM:
		RAV = getVAForm(state.excAddr, true);
		break;

	case IPR_IVPTBR:
		RAV = state.ivptb;
		break;

	case IPR_ICPERR_STAT:
		break;

	case IPR_PMCTR:
		break;

	case IPR_MM_STAT:
		RAV = state.mmstat;
		break;

	case IPR_VA:
		RAV = state.fvAddr;
		break;

	case IPR_VA_FORM:
		RAV = getVAForm(state.fvAddr, false);
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
	case IPR_ITB_TAG:
		state.tbvAddr = RBV;
		break;

	case IPR_ITB_PTE:
		addITBEntry(state.tbvAddr, RBV);
		break;

	case IPR_ITB_ASN:
		state.asn = (RBV >> 4) & 0x7F;
		break;

	case IPR_ITB_IA:
		tbia(ACC_EXEC);
		break;

	case IPR_ITB_IAP:
		tbiap(ACC_EXEC);
		break;

	case IPR_ITB_IS:
		tbis(RBV, ACC_EXEC);
		break;

	case IPR_SIRR:
		state.sir = (RBV >> 3) & 0xFFFE;
		break;

	case IPR_ASTRR:
		state.astrr = RBV & 0xF;
		break;

	case IPR_ASTER:
		state.aster = RBV ^ 0xF;
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
		state.icm = (RBV >> 3) & 3;
		break;

	case IPR_ICSR: // Ibox Control and Status Register
		ev5.icsr = (RBV & ICSR_RW) | ICSR_MBO;

		// Update internal registers
		state.sde  = (ev5.icsr & ICSR_SDE) ? 1 : 0;
		state.fpen = (ev5.icsr & ICSR_FPE) ? 1 : 0;
		state.ispe = (ev5.icsr & ICSR_SPE) >> 28;

		break;

	case IPR_IPLR:
		state.iplr = RBV & 0x1F;
		break;

	case IPR_IVPTBR:
//		state.ivptb = RBV & 0xFFFFFFFE0000000LL; // NT mode
		state.ivptb = RBV & 0xFFFFFFFFC000000LL;
		break;

	case IPR_HWINT_CLR:
		state.pcr  &= ~((RBV >> 27) & 7);
		state.crr  &= ~((RBV >> 32) & 1);
		state.slr  &= ~((RBV >> 33) & 1);
//		state.sisr &= ~((RBV >> 32) & 1);
		break;

	case IPR_IC_FLUSH_CTL: // Flush ICache
//		flushICache();
		break;

	case IPR_DTB_TAG:
		state.tbvAddr = RBV;
		break;

	case IPR_DTB_PTE:
		addDTBEntry(state.tbvAddr, RBV);
		break;

	case IPR_DTB_ASN:
		state.asn = RBV >> 56;
		break;

	case IPR_DTB_CM:
		state.dcm = (RBV >> 3) & 3;
		break;

	case IPR_MVPTBR:
//		state.dvptb = RBV & 0xFFFFFFFE0000000LL; // NT mode
		state.dvptb = RBV & 0xFFFFFFFFC000000LL;
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

// 0x100, "ISR","R","interrupt Serivce Request"
// 0x101, "ITB_TAG","W",   "Istream Translation Buffer Tag Register"
// 0x102, "ITB_PTE","R/W", "Instruction Translation Buffer Page Table Entry Register"
// 0x103, "ITB_ASN","R/W", "Instruction Translation Buffer Address Space Number Register"
// 0x104, "ITB_PTE_TEMP","R",
// 0x105, "ITB_IA","W",
// 0x106, "ITB_IAP","W",
// 0x107, "ITB_IS","W",
// 0x108, "SIRR","R/W",
// 0x109, "ASTRR","R/W",
// 0x10A, "ASTER","R/W",
// 0x10B, "EXC_ADDR","R/W",
// 0x10C, "EXC_SUM","R/W0C",
// 0x10D, "EXC_MASK","R",
// 0x10E, "PAL_BASE","R/W",
// 0x10F, "ICM","R/W",
// 0x110, "IPLR","R/W",
// 0x111, "INTID","R",
// 0x112, "IFAULT_VA_FORM","R",
// 0x113, "IVPTBR","R/W",
// 0x115, "HWINT_CLR","W",
// 0x116, "SL_XMIT","W",
// 0x117, "SL_RCV","R",
// 0x118, "ICSR","R/W",
// 0x119, "IC_FLUSH_CTL","W",
// 0x11A, "ICPERR_STAT","R/W1C",
// 0x11C, "PMCTR","R/W",
//
// 0x140, "PALtemp0","R/W", "PAL temp #0"
// 0x141, "PALtemp1","R/W", "PAL temp #1"
// 0x142, "PALtemp2","R/W", "PAL temp #2"
// 0x143, "PALtemp3","R/W", "PAL temp #3"
// 0x144, "PALtemp4","R/W", "PAL temp #4"
// 0x145, "PALtemp5","R/W", "PAL temp #5"
// 0x146, "PALtemp6","R/W", "PAL temp #6"
// 0x147, "PALtemp7","R/W", "PAL temp #7"
// 0x148, "PALtemp8","R/W", "PAL temp #8"
// 0x149, "PALtemp9","R/W", "PAL temp #9"
// 0x14A, "PALtemp10","R/W","PAL temp #10"
// 0x14B, "PALtemp11","R/W","PAL temp #11"
// 0x14C, "PALtemp12","R/W","PAL temp #12"
// 0x14D, "PALtemp13","R/W","PAL temp #13"
// 0x14E, "PALtemp14","R/W","PAL temp #14"
// 0x14F, "PALtemp15","R/W","PAL temp #15"
// 0x150, "PALtemp16","R/W","PAL temp #16"
// 0x151, "PALtemp17","R/W","PAL temp #17"
// 0x152, "PALtemp18","R/W","PAL temp #18"
// 0x153, "PALtemp19","R/W","PAL temp #19"
// 0x154, "PALtemp20","R/W","PAL temp #20"
// 0x155, "PALtemp21","R/W","PAL temp #21"
// 0x156, "PALtemp22","R/W","PAL temp #22"
// 0x157, "PALtemp23","R/W","PAL temp #23"
//
// 0x200, "DTB_ASN","W",
// 0x201, "DTB_CM","W",
// 0x202, "DTB_TAG","W",
// 0x203, "DTB_PTE","R/W",
// 0x204, "DTB_PTE_TEMP","R",
// 0x205, "MM_STAT","R",
// 0x206, "VA","R",
// 0x207, "VA_FORM", "R",
// 0x208, "MVPTBR","W",
// 0x209, "DTB_IAP","W",
// 0x20A, "DTB_IA","W",
// 0x20B, "DTB_IS","W",
// 0x20C, "ALT_MODE","W",
// 0x20D, "CC","W",
// 0x20E, "CC_CTL","W",
// 0x20F, "MCSR","R/W",
// 0x210, "DC_FLUSH","W",
// 0x212, "DC_PERR_STAT","R/W1C",
// 0x213, "DC_TEST_CTL","R/W",
// 0x214, "DC_TEST_TAG","R/W",
// 0x215, "DC_TEST_TAG_TEMP","R/W",
// 0x216, "DC_MODE","R/W",
// 0x217, "MAF_MODE","R/W",

//ctag_t iprNames[] =
//{
//		"ISR",           "ITB_TAG",       "ITB_PTE",     "ITB_ASN",             // 100-103
//		"ITB_PTE_TEMP",  "ITB_IA",        "ITB_IAP",     "ITB_IS",              // 104-107
//		"SIRR",          "ASTRR",         "ASTER",       "EXC_ADDR",            // 108-10B
//		"EXC_SUM",       "EXC_MASK",      "PAL_BASE",    "ICM",                 // 10C-10F
//		"IPLR",          "INTID",         "IVA_FORM",    "IVPTB",               // 110-113
//		nullptr,         "HWINT_CLR",     "SL_XMIT",     "SL_RCV",              // 114-117
//		"ICSR",			 "IC_FLUSH_CTL",  "ICPERR_STAT", nullptr,               // 118-11B
//		"PMCTR",        nullptr,          nullptr,       nullptr,               // 11C-11F
//
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 120-127
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 127-12F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 130-137
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 137-13F
//
//		"PALtemp0",   "PALtemp1",   "PALtemp2",   "PALtemp3",                   // 140-143
//		"PALtemp4",   "PALtemp5",   "PALtemp6",   "PALtemp7",                   // 144-147
//		"PALtemp8",   "PALtemp9",   "PALtemp10",  "PALtemp11",                  // 148-14B
//		"PALtemp12",  "PALtemp13",  "PALtemp14",  "PALtemp15",                  // 14C-14F
//		"PALtemp16",  "PALtemp17",  "PALtemp18",  "PALtemp19",                  // 150-153
//		"PALtemp20",  "PALtemp21",  "PALtemp22",  "PALtemp23",                  // 154-157
//
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 158-15F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 160-167
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 168-16F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 170-177
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 178-17F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 180-187
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 188-18F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 190-197
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 198-19F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 1A0-1A7
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 1A8=1AF
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 1B0=1B7
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 1B8-1Bf
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 1C0-1C7
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 1C8-1CF
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 1D0-1D7
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 1D8=1DF
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 1E0=1E7
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 1E8-1EF
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 1F0-1F7
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 1F8-1FF
//
//		"DTB_ASN",      "DTB_CM",            "DTB_TAG",        "DTB_PTE",       // 200-203
//		"DTB_PTE_TEMP", "MM_STAT",           "VA",             "VA_FORM",       // 204-207
//		"MVPTBR",       "DTB_IAP",           "DTB_IA",         "DTB_IS",        // 208-20B
//		"ALT_MODE",     "CC",                "CC_CTL",         "MCSR",          // 20C-20F
//		"DC_FLUSH",     nullptr,             "DC_PERR_STAT",   "DC_TEST_CTL",   // 210-213
//		"DC_TEST_TAG",  "DC_TEST_TAG_TEMP",  "DC_MODE",        "MAF_MODE",      // 214-217
//
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 218-21F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 220-227
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 228-22F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 230-237
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 238-23F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 240-247
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 248-24F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 250-257
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 258-25F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 260-267
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 268-26F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 270-277
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 278-27F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 280-287
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 288-28F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 290-297
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 298-29F
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 2A0-2A7
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 2A8-2AF
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 2B0-2B7
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 2B8-2BF
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 2C0-2C7
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 2C8-2CF
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 2D0-2D7
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 2D8-2DF
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 2E0-2E7
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 2E8-2EF
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 2F0-2F7
//		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // 2F8-2FF
//};
