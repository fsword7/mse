/*
 * sysconfig.cpp - system configuration package
 *
 *  Created on: Jul 14, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/driver.h"
#include "emu/sysconfig.h"

SystemConfig::SystemConfig(const SystemDriver &driver)
: sysDriver(driver)
{
	// Create root of system device
	systemDevice = addDeviceType(driver.name, driver.type, 0);

	systemDevice->completeConfig();
}

device_t *SystemConfig::addDeviceType(tag_t *tag, const DeviceType &type, uint64_t clock)
{

	device_t *sys = type.create(*this, tag, nullptr, clock);

	return sys;
}
