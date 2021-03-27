/*
 * mcs48.cpp - Intel MCS-48 microcomputer emulation package
 *
 *  Created on: Feb 14, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/addrmap.h"
#include "emu/map/map.h"
#include "emu/device.h"
#include "emu/sysconfig.h"

#include "dev/cpu/mcs48/mcs48.h"

using namespace aspace;

mcs48_cpuDevice::mcs48_cpuDevice(const SystemConfig &config, const DeviceType &type,
	const string &tagName, Device *owner, uint64_t clock, int paWidth, int daWidth)
: cpuDevice(config, type, tagName, owner, clock),
  mapProgramConfig("program", LittleEndian, 8, 16, 8, paWidth, 16, 0),
  mapDataConfig("data", LittleEndian, 8, 16, 8, daWidth, 16, 0),
  mapPortConfig("port", LittleEndian, 8, 16, 8, 8, 16, 0)
{
	// Initialize opcode table for disassembler
	initOpcodeTable();
}

mapConfigList mcs48_cpuDevice::getAddressConfigList() const
{
	return mapConfigList {
		{ aspace::asProgram, &mapProgramConfig },
		{ aspace::asData,    &mapDataConfig    },
		{ aspace::asPort,    &mapPortConfig    }
	};
}

void mcs48_cpuDevice::setProgram1K(AddressList &map)
{
	map(0x000, 0x3FF).rom().region("main");
}

void mcs48_cpuDevice::setProgram2K(AddressList &map)
{
	map(0x000, 0x7FF).rom().region("main");
}

void mcs48_cpuDevice::setProgram4K(AddressList &map)
{
	map(0x000, 0xFFF).rom().region("main");
}

void mcs48_cpuDevice::setData64(AddressList &map)
{
	map(0x00, 0x3F).ram().region("data");
}

void mcs48_cpuDevice::setData128(AddressList &map)
{
	map(0x00, 0x7F).ram().region("data");
}

void mcs48_cpuDevice::setData256(AddressList &map)
{
	map(0x00, 0xFF).ram().region("data");
}


// **************************************************************

DEFINE_DEVICE_TYPE(i8035, i8035_cpuDevice, "i8035", "Intel 8035")
DEFINE_DEVICE_TYPE(i8039, i8039_cpuDevice, "i8039", "Intel 8039")
DEFINE_DEVICE_TYPE(i8040, i8040_cpuDevice, "i8040", "Intel 8039")
DEFINE_DEVICE_TYPE(i8048, i8048_cpuDevice, "i8048", "Intel 8048")
DEFINE_DEVICE_TYPE(i8049, i8049_cpuDevice, "i8049", "Intel 8049")
DEFINE_DEVICE_TYPE(i8050, i8050_cpuDevice, "i8050", "Intel 8050")
DEFINE_DEVICE_TYPE(i8749, i8749_cpuDevice, "i8749", "Intel 8749")
DEFINE_DEVICE_TYPE(i8750, i8750_cpuDevice, "i8750", "Intel 8750")

// 8035 with external ROM and 64 bytes RAM
i8035_cpuDevice::i8035_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8035, tagName, owner, clock, 10, 6)
{
}

// 8039 with external ROM and 128 bytes RAM
i8039_cpuDevice::i8039_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8039, tagName, owner, clock, 11, 7)
{
}

// 8040 with external ROM and 256 bytes RAM
i8040_cpuDevice::i8040_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8040, tagName, owner, clock, 12, 8)
{
}

// 8048 with 1K ROM and 64 bytes RAM
i8048_cpuDevice::i8048_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8048, tagName, owner, clock, 10, 6)
{
}

// 8049 with 2K ROM and 128 bytes RAM
i8049_cpuDevice::i8049_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8049, tagName, owner, clock, 11, 7)
{
}

// 8050 with 4K ROM and 256 bytes RAM
i8050_cpuDevice::i8050_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8050, tagName, owner, clock, 12, 8)
{
}

// 8749 with 2K EPROM and 128 bytes RAM
i8749_cpuDevice::i8749_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8749, tagName, owner, clock, 11, 7)
{
}

// 8750 with 4K EPROM and 256 bytes RAM
i8750_cpuDevice::i8750_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: mcs48_cpuDevice(config, i8750, tagName, owner, clock, 12, 8)
{
}
