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
