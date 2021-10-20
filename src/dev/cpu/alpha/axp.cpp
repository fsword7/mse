/*
 * alpha.cpp - DEC Alpha CPU Processor package
 *
 *  Created on: Nov 20, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/sysconfig.h"
#include "dev/cpu/alpha/axp.h"

alpha_cpuDevice::alpha_cpuDevice(const SystemConfig &config, const DeviceType &type,
	const string &tagName, Device *owner, uint64_t clock, int aWidth)
: cpuDevice(config, type, tagName, owner, clock),
  mapProgramConfig("program", LittleEndian, 64, 16, 8, aWidth, 16, 0)
{
	// Initialize opcode table for disassembler
	initOpcodeTable();
}

mapConfigList alpha_cpuDevice::getAddressConfigList() const
{
	return mapConfigList {
		{ aspace::asProgram, &mapProgramConfig }
	};
}

void alpha_cpuDevice::startDevice()
{

	getAddressSpace(AS_PROGRAM)->setMemorySpecific(mapProgram);
	
}