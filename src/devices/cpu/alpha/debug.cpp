/*
 * debug.cpp - Alpha CPU Processor - debugging routines
 *
 *  Created on: Nov 21, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/didebug.h"
#include "devices/cpu/alpha/axp.h"
#include "devices/cpu/alpha/debug.h"

string AlphaProcessor::getStringAddress(offs_t addr)
{
	string strAddr;

	strAddr = fmt::sprintf("%04X.%04X.%04X.%04X",
		uint16_t(addr >> 48), uint16_t(addr >> 32),
		uint16_t(addr >> 16), uint16_t(addr));

	return strAddr;
}

int AlphaProcessor::list(Console *cty, offs_t vAddr)
{
	using namespace aspace;

	AddressSpace *space = getAddressSpace(AS_PROGRAM);
	uint32_t inCode, opCode, fnCode;
	string line;

	inCode = space->read32(vAddr);
	opCode = OP_GETCD(inCode);

	fmt::printf("%s %08X <Opcode %02X>\n", getStringAddress(vAddr), inCode, opCode);

	// advance next PC address
	return 4;
}
