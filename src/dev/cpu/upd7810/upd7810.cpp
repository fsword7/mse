/*
 * upd7810.cpp - NEC uPD7810 series emulation package
 *
 *  Created on: Mar 29, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/addrmap.h"
#include "emu/map/map.h"
#include "emu/device.h"
#include "emu/sysconfig.h"

#include "dev/cpu/upd7810/upd7810.h"

using namespace aspace;

upd7810_cpuDevice::upd7810_cpuDevice(const SystemConfig &config, const DeviceType &type,
	const string &tagName, Device *owner, uint64_t clock)
: cpuDevice(config, type, tagName, owner, clock),
  mapProgramConfig("program", LittleEndian, 8, 16, 8, 16, 16, 0)
{
	// Initialize opcode table for disassembler
//	initOpcodeTable();
}

mapConfigList upd7810_cpuDevice::getAddressConfigList() const
{
	return mapConfigList {
		{ aspace::asProgram, &mapProgramConfig },
	};
}


// **************************************************************

DEFINE_DEVICE_TYPE(upd7810, upd7810_cpuDevice, "upd7810", "NEC uPD7810")

upd7810_cpuDevice::upd7810_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: upd7810_cpuDevice(config, upd7810, tagName, owner, clock)
{
	// Initialize opcode table for disassembler
//	initOpcodeTable();
}
