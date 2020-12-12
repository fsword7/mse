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
	tsunami_device(const SystemConfig &config, const DeviceType &type)
	: SystemDevice(config, type)
	{

	}

	// System creator routines
	void es40(SystemConfig &config);

	// Model-specific system initialize routines
	static void es40_init();

private:
	AlphaProcessor *cpu[ES40_NCPU];
};
