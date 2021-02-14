/*
 * devsys.cpp - system device package
 *
 *  Created on: Jul 14, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/dibus.h"
#include "emu/devsys.h"

sysDevice::sysDevice(const SystemConfig &config, const DeviceType &type, cstag_t &tagName, uint64_t clock)
: Device(config, type, tagName, nullptr, clock),
  driver(config.getSystemDriver())
{

}

void sysDevice::devConfigure(SystemConfig &config)
{
	assert(&config == &getSystemConfig());

	// system-specific initialization
	driver.configure(config, *this);
}
