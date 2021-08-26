/*
 * debug.cpp - Alpha CPU Processor - debugging routines
 *
 *  Created on: Nov 21, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/didebug.h"
#include "dev/cpu/alpha/axp.h"
#include "dev/cpu/alpha/debug.h"
#include "dev/cpu/alpha/opcodes.h"

string alpha_cpuDevice::getStringAddress(offs_t addr)
{
	string strAddr;

	strAddr = fmt::sprintf("%04X.%04X.%04X.%04X",
		uint16_t(addr >> 48), uint16_t(addr >> 32),
		uint16_t(addr >> 16), uint16_t(addr));

	return strAddr;
}

int alpha_cpuDevice::list(Console *cty, offs_t vAddr)
{
	uint32_t opWord, opc, fnc;
	uint32_t mdpAddr;
	int rb;
	string line, oprLine;

	opcAlpha *opCode;

	opWord = mapProgram.read32(vAddr, nullptr);
	opc = OP_GETOP(opWord);

	switch (opc)
	{
	case 0x10: opCode = axpCodes10[OP_GETFNC(opWord) & 0x7F];  break;
	case 0x11: opCode = axpCodes11[OP_GETFNC(opWord) & 0x7F];  break;
	case 0x12: opCode = axpCodes12[OP_GETFNC(opWord) & 0x7F];  break;
	case 0x13: opCode = axpCodes13[OP_GETFNC(opWord) & 0x7F];  break;
	case 0x14: opCode = axpCodes14[OP_GETFNC(opWord) & 0x7F];  break;
	case 0x15: opCode = axpCodes15[OP_GETFNC(opWord) & 0x7FF]; break;
	case 0x16: opCode = axpCodes16[OP_GETFNC(opWord) & 0x7FF]; break;
	case 0x17: opCode = axpCodes17[OP_GETFNC(opWord) & 0x7FF]; break;
	case 0x18: opCode = axpCodes18[(opWord >> 8) & 0xFF];      break;
	case 0x1A: opCode = axpCodes1A[OP_GETJMP(opWord)];         break;
	case 0x1C: opCode = axpCodes1C[OP_GETFNC(opWord) & 0x7F];  break;
	default:   opCode = axpCodes[opc]; break;
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
		oprLine += fmt::sprintf("r%d", OP_GETRA(opWord));

	if (opFlags & OPR_BDP)
	{
		if (oprLine.size() > 0)
			oprLine += ",";
		oprLine += fmt::sprintf("%llX", vAddr + (OP_GETBDP(opWord) << 2) + 4);
	}
	else if (opFlags & OPR_MDP)
	{
		if (oprLine.size() > 0)
			oprLine += ",";
		mdpAddr = OP_GETMDP(opWord) & 0xFFFF;
		if ((rb = OP_GETRB(opWord)) < REG_ZERO)
			oprLine += fmt::sprintf("%04X(r%d)", mdpAddr, rb);
		else
			oprLine += fmt::sprintf("%04X", mdpAddr);
	}
	else if (opFlags & OPR_RB)
	{
		rb = OP_GETRB(opWord) & 0x1F;
		if (opFlags & OPR_IDX)
			oprLine += fmt::sprintf("(r%d)", rb);
		else
		{
			if (oprLine.size() > 0)
				oprLine += ",";
			if ((opFlags & OPR_LIT) && (opWord & OPC_LIT))
				oprLine += fmt::sprintf("#%02X", OP_GETLIT(opWord));
			else
				oprLine += fmt::sprintf("r%d", rb);
		}
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

void alpha_cpuDevice::initOpcodeTable()
{

	uint32_t opFlags;
	int opCode, fnCode;

	// Clear all opcode tables
	for (int idx = 0; idx < 64; idx++)
		axpCodes[idx] = nullptr;
	for (int idx = 0; idx < 128; idx++)
		axpCodes10[idx] = nullptr;
	for (int idx = 0; idx < 128; idx++)
		axpCodes11[idx] = nullptr;
	for (int idx = 0; idx < 128; idx++)
		axpCodes12[idx] = nullptr;
	for (int idx = 0; idx < 128; idx++)
		axpCodes13[idx] = nullptr;
	for (int idx = 0; idx < 128; idx++)
		axpCodes14[idx] = nullptr;
	for (int idx = 0; idx < 2048; idx++)
		axpCodes15[idx] = nullptr;
	for (int idx = 0; idx < 2048; idx++)
		axpCodes16[idx] = nullptr;
	for (int idx = 0; idx < 2048; idx++)
		axpCodes17[idx] = nullptr;
	for (int idx = 0; idx < 256; idx++)
		axpCodes18[idx] = nullptr;
	for (int idx = 0; idx < 4; idx++)
		axpCodes1A[idx] = nullptr;
	for (int idx = 0; idx < 128; idx++)
		axpCodes1C[idx] = nullptr;

	for (int idx = 0; axp_opCodes[idx].opName; idx++)
	{
		opFlags = axp_opCodes[idx].opFlags;
		opCode  = axp_opCodes[idx].opCode;
		fnCode  = axp_opCodes[idx].fnCode;

		switch (opCode)
		{
		case 0x10: axpCodes10[fnCode] = &axp_opCodes[idx]; break;
		case 0x11: axpCodes11[fnCode] = &axp_opCodes[idx]; break;
		case 0x12: axpCodes12[fnCode] = &axp_opCodes[idx]; break;
		case 0x13: axpCodes13[fnCode] = &axp_opCodes[idx]; break;
		case 0x14: axpCodes14[fnCode] = &axp_opCodes[idx]; break;
		case 0x15: axpCodes15[fnCode] = &axp_opCodes[idx]; break;
		case 0x16: axpCodes16[fnCode] = &axp_opCodes[idx]; break;
		case 0x17: axpCodes17[fnCode] = &axp_opCodes[idx]; break;
		case 0x18: axpCodes18[fnCode >> 8] = &axp_opCodes[idx]; break;
		case 0x1A: axpCodes1A[fnCode] = &axp_opCodes[idx]; break;
		case 0x1C: axpCodes1C[fnCode] = &axp_opCodes[idx]; break;
		default:   axpCodes[opCode]   = &axp_opCodes[idx]; break;
		}
	}
}
