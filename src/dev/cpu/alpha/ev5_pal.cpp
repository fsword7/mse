/*
 * ev5_pal.cpp - 21164 series - EV5 PAL hardware instructions
 *
 *  Created on: Feb 6, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/alpha/ev5.h"

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

	}

}

void dec21164_cpuDevice::hw_mtpr(uint32_t opWord)
{
	int fnc = opWord & 0xFFFF;
	uint64_t rbv;

	switch (fnc)
	{
	case IPR_PAL_BASE: // PAL base address register
		state.palBase = RBV & PAL_BASE_MASK;
		break;
	}
}
