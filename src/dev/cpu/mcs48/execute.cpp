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
}

void mcs48_cpuDevice::setPCAddress(offs_t addr)
{
//	state.pcAddr = addr;
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

}
