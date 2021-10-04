/*
 * i8080.cpp - Intel 8080/8085 processor emulation package
 *
 *  Created on: Jun 24, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/i8080/i8080.h"

i8080_cpuDevice::i8080_cpuDevice(const SystemConfig &config, const DeviceType &type,
	const string &tagName, Device *owner, uint64_t clock, cpuType idType)
: cpuDevice(config, type, tagName, owner, clock),
  mapProgramConfig("program", LittleEndian, 8, 16, 8, 16, 16, 0),
  mapIOPortConfig("I/O", LittleEndian, 8, 16, 8, 8, 16, 0),
  idProcessorType(idType)
{
	// Initialize opcode table for disassembler
//	initOpcodeTable();
}

mapConfigList i8080_cpuDevice::getAddressConfigList() const
{
	return mapConfigList {
		{ aspace::asProgram, &mapProgramConfig },
		{ aspace::asPort,    &mapIOPortConfig  }
	};
}

void i8080_cpuDevice::startDevice()
{
	// Test state registers (to be removed later)
	// bcReg.uw = 0x1234;
	// deReg.uw = 0x5678;
	// afReg.ub.l = 0xA5;

	// Register CPU registers for debugging purposes
	{
		addState(i8080_AF, "AF",  afReg.uw);
		addState(i8080_BC, "BC",  bcReg.uw);
		addState(i8080_DE, "DE",  deReg.uw);
		addState(i8080_HL, "HL",  hlReg.uw);
		addState(i8080_PC, "PC",  pcReg.uw);
		addState(i8080_SP, "SP",  spReg.uw);
		
		addState(i8080_B,  "B",   bcReg.ub.h);
		addState(i8080_C,  "C",   bcReg.ub.l);
		addState(i8080_D,  "D",   deReg.ub.h);
		addState(i8080_E,  "E",   deReg.ub.l);
		addState(i8080_H,  "H",   hlReg.ub.h);
		addState(i8080_L,  "L",   hlReg.ub.l);
		addState(i8080_A,  "A",   afReg.ub.h);
		addState(i8080_F,  "F",   afReg.ub.l);

		// add current PC and status registers
		addState(STATE_GENFLAGS, "CURFLAGS", afReg.ub.l).noshow().setFormat("%8s");
		addState(STATE_GENPC, "CURPC", pcReg.uw).noshow();
		addState(STATE_GENPCBASE, "CURPCBASE", pcBase).noshow();

		// addState(i8080_B,  "B",   iRegs[REG_B]);
		// addState(i8080_C,  "C",   iRegs[REG_C]);
		// addState(i8080_D,  "D",   iRegs[REG_D]);
		// addState(i8080_E,  "E",   iRegs[REG_E]);
		// addState(i8080_H,  "H",   iRegs[REG_H]);
		// addState(i8080_L,  "L",   iRegs[REG_L]);
		// addState(i8080_A,  "A",   iRegs[REG_A]);
		// addState(i8080_F,  "F",   iRegs[REG_F]);

		// Initialize executable parameters
		init();
	}

	// Assigns system bus access spaces
	getAddressSpace(AS_PROGRAM)->setMemorySpecific(mapProgram);
	getAddressSpace(AS_IOPORT)->setMemorySpecific(mapIOPort);

	setCycleCounter(&opCounter);
}

void i8080_cpuDevice::exportString(const DeviceStateEntry &entry, string &out) const
{
	switch (entry.getIndex())
	{
		case STATE_GENFLAGS:
			out = fmt::sprintf("%c%c%c%c%c%c%c%c",
				afReg.ub.l & 0x80 ? 'S' : '-',
				afReg.ub.l & 0x40 ? 'Z' : '-',
				afReg.ub.l & 0x20 ? 'X' : '-',
				afReg.ub.l & 0x10 ? 'H' : '-',
				afReg.ub.l & 0x08 ? '?' : '-',
				afReg.ub.l & 0x04 ? 'P' : '-',
				afReg.ub.l & 0x02 ? 'V' : '-',
				afReg.ub.l & 0x01 ? 'C' : '-');
			break;
	}
}

void i8080_cpuDevice::enableInterrupts(bool sw)
{
    
}


DEFINE_DEVICE_TYPE(i8080,  i8080_cpuDevice,  "i8080",  "Intel 8080")
DEFINE_DEVICE_TYPE(i8080a, i8080a_cpuDevice, "i8080A", "Intel 8080A")
DEFINE_DEVICE_TYPE(i8085a, i8085a_cpuDevice, "i8085A", "Intel 8085A")


// Intel 8080 processor
i8080_cpuDevice::i8080_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: i8080_cpuDevice(config, i8080, tagName, owner, clock, cpuid_8080)
{
}

// Intel 8080A processor
i8080a_cpuDevice::i8080a_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: i8080_cpuDevice(config, i8080a, tagName, owner, clock, cpuid_8080A)
{
}

// Intel 8085A processor
i8085a_cpuDevice::i8085a_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: i8080_cpuDevice(config, i8085a, tagName, owner, clock, cpuid_8085A)
{
}
