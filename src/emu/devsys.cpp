/*
 * devsys.cpp - system device package
 *
 *  Created on: Jul 14, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/devsys.h"

SystemDevice::SystemDevice(const SystemConfig &config, const DeviceType &type)
: Device(config, type), driver(config.getSystemDriver())
{

}

void SystemDevice::devConfigure(SystemConfig &config)
{
	assert(&config == &getSystemConfig());

	// system-specific initialization
	driver.configure(config, *this);
}
