/*
 * alphapc.h - AlphaPC 164 systems
 *
 *  Created on: Feb 7, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/devsys.h"
#include "dev/cpu/alpha/ev5.h"

class alphapc_sysDevice : public SystemDevice
{
public:
	alphapc_sysDevice(const SystemConfig &config, const DeviceType &type, cstag_t &tagName, uint64_t clock)
	: SystemDevice(config, type, tagName, clock),
	  config("system", LittleEndian, 64, 16, 8, 40, 16, 0)
	{

	}
	~alphapc_sysDevice() = default;

	// System creator routines
	void pc164(SystemConfig &config);
	void pc164lx(SystemConfig &config);
	void pc164sx(SystemConfig &config);

	// Model-specific system initialize routines
	static void pc164_init();
	static void pc164lx_init();
	static void pc164sx_init();

	void pc164_sbus(aspace::AddressList &map);
	void pc164lx_sbus(aspace::AddressList &map);
	void pc164sx_sbus(aspace::AddressList &map);

private:
	dec21164_cpuDevice *cpu = nullptr;

	mapAddressConfig config;
};
