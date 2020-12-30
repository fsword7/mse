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
	: SystemDevice(config, type, tagName, clock)
	{

	}
	~tsunami_device() = default;

	// System creator routines
	void es40(SystemConfig &config);

	// Model-specific system initialize routines
	static void es40_init();

	uint8_t read8(offs_t addr);
	void write8(offs_t addr, uint8_t data);

private:
	AlphaProcessor *cpu[ES40_NCPU];
};
