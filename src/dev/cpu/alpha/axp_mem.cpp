/*
 * axp_mem.cpp - Alpha processor - memory management
 *
 *  Created on: Feb 7, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/alpha/axp.h"

// Virtual address to physical conversion
uint64_t AlphaProcessor::probev(uint64_t vAddr, uint32_t flags, bool &asmb, int &status)
{
//	int spe = (flags & ACC_EXEC) ? state.ispe : state.mspe;
//	int cm  = (flags & ALTCM) ? state.altcm : state.cm;

	asmb   = true;
	status = 0;
	return vAddr;
}

uint64_t AlphaProcessor::readv(uint64_t vAddr, int size)
{
	return 0;
}

void AlphaProcessor::writev(uint64_t vAddr, uint64_t data, int size)
{
}
