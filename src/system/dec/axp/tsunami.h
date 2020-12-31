/*
 * tsunami.h - AlphaServer systems (tsunami family)
 *
 *  Created on: Nov 22, 2020
 *      Author: Tim Stark
 */

#pragma once

#define ES40_NCPU	4

class tsunami_device : public SystemDevice
{
public:
	tsunami_device(const SystemConfig &config, const DeviceType &type, cstag_t &tagName, uint64_t clock)
	: SystemDevice(config, type, tagName, clock),
	  config("system", LittleEndian, 64, 44, 0)
	{

	}
	~tsunami_device() = default;

	// System creator routines
	void es40(SystemConfig &config);

	// Model-specific system initialize routines
	static void es40_init();

private:
	AlphaProcessor *cpu[ES40_NCPU];

	mapAddressConfig config;
	mapAddressSpace *space = nullptr; // program space
};
