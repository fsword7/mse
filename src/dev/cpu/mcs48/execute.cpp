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

	mapProgram = getAddressSpace(AS_PROGRAM);
	mapData = getAddressSpace(AS_DATA);
	mapPort = getAddressSpace(AS_IOPORT);
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

void mcs48_cpuDevice::updateRegisters()
{
	gReg = &data[pswReg & PSW_B ? 24 : 0];
}

void mcs48_cpuDevice::executeJcc(bool flag)
{
	uint16_t pch = pcAddr & 0xF00;
	uint8_t  offset = fetchi();
	if (flag == true)
		pcAddr = pch | offset;
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

	case 0x14: case 0x34: case 0x54: case 0x74:
	case 0x94: case 0xB4: case 0xD4: case 0xE4:
		// CALL instruction
		chargeCycles(2);
		newAddr = ((uint16_t(opCode) << 3) & 0x700) | fetchi();
		pcAddr  = (newAddr & 0x7FF) | (pcAddr & 0x800);
		return;

	case 0x27:
		// CLR A instruction
		chargeCycles(1);
		aReg = 0;
		return;

	case 0x97:
		// CLR C instruction
		chargeCycles(1);
		pswReg &= ~PSW_C;
		return;

	case 0x95:
		// CLR F0 instruction
		chargeCycles(1);
		pswReg &= ~PSW_F;
		return;

	case 0xB5:
		// CLR F1 instruction
		chargeCycles(1);
		f1Flag = false;
		return;



	case 0x12: case 0x32: case 0x52: case 0x72:
	case 0x92: case 0xB2: case 0xD2: case 0xE2:
		// JBx address instruction
		chargeCycles(2);
		executeJcc(aReg & (1u << (opCode >> 5)));
		return;


	case 0x00:
		// NOP instruction
		chargeCycles(1);
		// Do nothiing
		return;

	case 0xE5:
		// SEL MB0 instruction
		chargeCycles(1);
		a11Addr = 0x000;
		return;

	case 0xF5:
		// SEL MB1 instruction
		chargeCycles(1);
		a11Addr = 0x800;
		return;

	case 0xC5:
		// SEL RB0 instruction
		chargeCycles(1);
		pswReg &= ~PSW_B;
		updateRegisters();
		return;

	case 0xD5:
		// SEL RB1 instruction
		chargeCycles(1);
		pswReg |= PSW_B;
		updateRegisters();
		return;

	default:
		dbg.log("*** Unimplemented opcode = %02X\n", opCode);
		return;
	}
}
