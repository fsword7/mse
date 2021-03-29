/*
 * debug.cpp - NEC uPD7810 series - debugging routines
 *
 *  Created on: Mar 29, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/didebug.h"
#include "dev/cpu/upd7810/upd7810.h"

string upd7810_cpuDevice::getStringAddress(offs_t addr)
{
	string strAddr;

	// Display 16-bit address
	strAddr = fmt::sprintf("%04X", addr & 0xFFFF);

	return strAddr;
}

int upd7810_cpuDevice::list(Console *cty, offs_t vAddr)
{
	using namespace aspace;

	return 0;
}

void upd7810_cpuDevice::initOpcodeTable()
{
}
