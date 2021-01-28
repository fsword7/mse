/*
 * alpha.cpp - DEC Alpha CPU Processor package
 *
 *  Created on: Nov 20, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/device.h"
#include "emu/sysconfig.h"
#include "emu/map/map.h"
#include "dev/cpu/alpha/axp.h"

AlphaProcessor::AlphaProcessor(const SystemConfig &config, const DeviceType &type,
	const string &tagName, Device *owner, uint64_t clock, int aWidth)
: ProcessorDevice(config, type, tagName, owner, clock),
  mapProgramConfig("program", LittleEndian, 64, 16, 8, aWidth, 16, 0)
{
	// Initialize opcode table for disassembler
	initOpcodeTable();
}

mapConfigList AlphaProcessor::getAddressConfigList() const
{
	return mapConfigList {
		{ aspace::asProgram, &mapProgramConfig }
	};
}
