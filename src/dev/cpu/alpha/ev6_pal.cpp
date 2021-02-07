/*
 * ev6arch.cpp - EV6 architecture (21164 series)
 *
 *  Created on: Feb 5, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/alpha/axp.h"
#include "dev/cpu/alpha/ev6_ipr.h"

void AlphaProcessor::hw_mfpr(uint32_t opWord)
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
	}
}

void AlphaProcessor::hw_mtpr(uint32_t opWord)
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
		break;

	case IPR_IC_FLUSH_ASM: // IC_FLUSH_ASM
//		flushICacheASM();
		break;

	case IPR_IC_FLUSH: // IC_FLUSH
//		flushICache();
		break;
	}
}



