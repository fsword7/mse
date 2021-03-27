/*
 * execute.cpp - MCS-48 microprocessor - execute routines
 *
 *  Created on: Mar 26, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/mcs48/mcs48.h"

void mcs48_cpuDevice::init()
{
	pcMask = 0x7FF;

	mapProgram = getAddressSpace(AS_PROGRAM);
	mapData = getAddressSpace(AS_DATA);
//	mapPort = getAddressSpace(AS_PORT);
}

void mcs48_cpuDevice::setPCAddress(offs_t addr)
{
//	state.pcAddr = addr;
}

uint8_t mcs48_cpuDevice::fetchi()
{
	uint16_t addr = pcAddr;
	pcAddr = ((pcAddr + 1) & 0x7FF) | (pcAddr & 0x800);
	return mapProgram->read8(addr);
}

void mcs48_cpuDevice::step(Console *user)
{
//	// Save current log flags and
//	// enable console output
//	uint32_t flags = dbg.getLogFlags();
//	dbg.setLogFlags(LOG_CONSOLE);
//
//	// Execute one instruction as single step
//	log = user;
//	execute();
//
//	// Restore log flags
//	dbg.loadLogFlags(flags);
}

void mcs48_cpuDevice::run()
{
//	// Start execution state
//	pState = execRunning;
//
//	try {
//		while(pState == execRunning)
//		{
//			execute();
//		}
//		dbg.flushAll();
//	}
//
//	catch (...)
//	{
//		// Flush all remaining buffers
//		dbg.flushAll();
//	}
//
//	// Stop execution state
//	pState = execStopped;
}

void mcs48_cpuDevice::execute()
{
	uint16_t newAddr;

	// Fetch instruction from instruction stream
	opCode = fetchi();

	switch(opCode)
	{
	case 0x00:
		// NOP instruction
		// Do nothing
		return;

	case 0x14: case 0x34: case 0x54: case 0x74:
	case 0x94: case 0xB4: case 0xD4: case 0xE4:
		// CALL instruction
		newAddr = ((uint16_t(opCode) << 3) & 0x700) | fetchi();
		pcAddr  = (newAddr & 0x7FF) | (pcAddr & 0x800);
		return;

	default:
		dbg.log("*** Unimplemented opcode = %02X\n", opCode);
		return;
	}
}
