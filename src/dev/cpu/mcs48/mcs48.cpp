/*
 * mcs48.cpp - Intel MCS-48 microcomputer emulation package
 *
 *  Created on: Feb 14, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/core.h"
#include "emu/device.h"
#include "emu/sysconfig.h"
#include "emu/map/map.h"

#include "dev/cpu/mcs48/mcs48.h"

mcs48_cpuDevice::mcs48_cpuDevice(const SystemConfig &config, const DeviceType &type,
	const string &tagName, Device *owner, uint64_t clock, int aWidth)
: cpuDevice(config, type, tagName, owner, clock),
  mapProgramConfig("program", LittleEndian, 8, 16, 8, aWidth, 16, 0),
  mapDataConfig("data", LittleEndian, 8, 16, 8, aWidth, 16, 0)
{
	// Initialize opcode table for disassembler
	initOpcodeTable();
}

mapConfigList mcs48_cpuDevice::getAddressConfigList() const
{
	return mapConfigList {
		{ aspace::asProgram, &mapProgramConfig },
		{ aspace::asData,    &mapDataConfig    }
	};
}

// **************************************************************

DEFINE_DEVICE_TYPE(mcs8035, mcs8035_cpuDevice, "8035", "MCS-48 8035 microcomputer")
DEFINE_DEVICE_TYPE(mcs8039, mcs8039_cpuDevice, "8039", "MCS-48 8039 microcomputer")
DEFINE_DEVICE_TYPE(mcs8040, mcs8040_cpuDevice, "8040", "MCS-48 8039 microcomputer")
DEFINE_DEVICE_TYPE(mcs8048, mcs8048_cpuDevice, "8048", "MCS-48 8048 microcomputer")
DEFINE_DEVICE_TYPE(mcs8049, mcs8049_cpuDevice, "8049", "MCS-48 8049 microcomputer")
DEFINE_DEVICE_TYPE(mcs8050, mcs8050_cpuDevice, "8050", "MCS-48 8050 microcomputer")
DEFINE_DEVICE_TYPE(mcs8749, mcs8749_cpuDevice, "8749", "MCS-48 8749 microcomputer")
DEFINE_DEVICE_TYPE(mcs8750, mcs8750_cpuDevice, "8750", "MCS-48 8750 microcomputer")

// 8035 with 64 bytes RAM (no internal program)
mcs8035_cpuDevice::mcs8035_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, mcs8035, tagName, owner, clock, 10)
{
}

// 8039 with 128 bytes RAM (no internal program)
mcs8039_cpuDevice::mcs8039_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, mcs8039, tagName, owner, clock, 11)
{
}

// 8040 with 256 bytes RAM (no internal program)
mcs8040_cpuDevice::mcs8040_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, mcs8040, tagName, owner, clock, 12)
{
}

// 8048 with 1K ROM and 64 bytes RAM
mcs8048_cpuDevice::mcs8048_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, mcs8048, tagName, owner, clock, 10)
{
}

// 8049 with 2K ROM and 128 bytes RAM
mcs8049_cpuDevice::mcs8049_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, mcs8049, tagName, owner, clock, 11)
{
}

// 8050 with 4K ROM and 256 bytes RAM
mcs8050_cpuDevice::mcs8050_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, mcs8050, tagName, owner, clock, 12)
{
}

// 8749 with 2K EPROM and 128 bytes RAM
mcs8749_cpuDevice::mcs8749_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, mcs8749, tagName, owner, clock, 11)
{
}

// 8750 with 4K EPROM and 256 bytes RAM
mcs8750_cpuDevice::mcs8750_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, mcs8750, tagName, owner, clock, 12)
{
}
