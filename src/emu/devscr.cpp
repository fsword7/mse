/*
 * devscr.cpp - screen device package for video controllers
 *
 *  Created on: Sep 13, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/devscr.h"



scrDevice::scrDevice(const SystemConfig &config, const DeviceType &type, cstag_t &tagName, uint64_t clock)
: Device(config, type, tagName, nullptr, clock),
  driver(config.getSystemDriver())
{

}

void scrDevice::devConfigure(SystemConfig &config)
{
	assert(&config == &getSystemConfig());

	// system-specific initialization
	driver.configure(config, *this);
}
