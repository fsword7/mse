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
#include "devices/cpu/alpha/opcodes.h"

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
	uint32_t opWord, opc, fnc;
	string line, oprLine;

	opcAlpha *opCode;

	opWord = space->read32(vAddr);
	opc = OP_GETCD(opWord);

	switch (opc)
	{
	default: opCode = axpCodes[opc];
	}

	line = fmt::sprintf("%s %08X  ", getStringAddress(vAddr), opWord);

	if (opCode == nullptr) {
		line += fmt::sprintf("<Unknown opcode %02X>", opc);
		cout << line << endl;
		return 4;
	}
	line += fmt::sprintf("%-10s", opCode->opName);
	uint32_t opFlags = opCode->opFlags;

	if (opFlags & OPR_RA)
		oprLine += fmt::sprintf("r%d", OP_GETRA(opWord) & 0x1F);

	if (opFlags & OPR_BDP)
	{
		if (oprLine.size() > 0)
			oprLine += ",";
		oprLine += fmt::sprintf("%llX", vAddr + (OP_GETBDSP(opWord) << 2) + 4);
	}

	if (opFlags & OPR_RC)
	{
		if (oprLine.size() > 0)
			oprLine += ',';
		oprLine += fmt::sprintf("r%d", OP_GETRC(opWord) & 0x1F);
	}

	fmt::printf("%s %s\n", line, oprLine);

	// advance next PC address
	return 4;
}

void AlphaProcessor::initOpcodeTable()
{

	uint32_t opFlags;
	int opCode, fnCode;

	// Clear all opcode tables
	for (int idx = 0; idx < OPM_CODE+1; idx++)
		axpCodes[idx] = nullptr;

	for (int idx = 0; axp_opCodes[idx].opName; idx++)
	{
		opFlags = axp_opCodes[idx].opFlags;
		opCode  = axp_opCodes[idx].opCode;
		fnCode  = axp_opCodes[idx].fnCode;

		switch (opCode)
		{
		default: axpCodes[opCode] = &axp_opCodes[idx];
		}
	}
}
