/*
 * vax.cpp - DEC VAX processor emulation package
 *
 *  Created on: Feb 14, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/vax/vax.h"

vax_cpuDevice::vax_cpuDevice(const SystemConfig &config, const DeviceType &type,
	const string &tagName, Device *owner, uint64_t clock, int aWidth)
: cpuDevice(config, type, tagName, owner, clock),
  mapProgramConfig("program", LittleEndian, 32, 16, 8, aWidth, 16, 0)
{
	// Initialize opcode table for disassembler
//	initOpcodeTable();
}
