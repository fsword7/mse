/*
 * mx.h - Epson MX80/MX100 printer emulation package
 *
 *  Created on: Mar 26, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/devsys.h"
#include "dev/cpu/mcs48/mcs48.h"

class mx_prDevice : public sysDevice
{
public:
	mx_prDevice(const SystemConfig &config, const DeviceType &type, cstag_t &tagName, uint64_t clock)
	: sysDevice(config, type, tagName, clock),
	  config("printer", LittleEndian, 8, 16, 8, 12, 16, 0)
	{

	}
	~mx_prDevice() = default;

	// System creator routines
	void mx80(SystemConfig &config);
	void mx100(SystemConfig &config);

	// Model-specific system initialize routines
	static void mx80_init();
	static void mx100_init();

	void mx_sbus(aspace::AddressList &map);

private:
	i8049_cpuDevice *cpu = nullptr;
//	i8041_cpuDevice *scpu = nullptr;

	mapAddressConfig config;
};
