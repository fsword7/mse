/*
 * execute.cpp - NEC uPD7810 series - execute routines
 *
 *  Created on: Mar 29, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/upd7810/upd7810.h"

void upd7810_cpuDevice::init()
{
	// Reset all memory configurations
//	mapProgram = getAddressSpace(AS_PROGRAM);
//	mapData = getAddressSpace(AS_DATA);
//	mapPort = getAddressSpace(AS_IOPORT);
}

void upd7810_cpuDevice::setPCAddress(offs_t addr)
{
//	pcAddr = addr;
}


void upd7810_cpuDevice::step(Console *user)
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

void upd7810_cpuDevice::run()
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

void upd7810_cpuDevice::execute()
{
}
