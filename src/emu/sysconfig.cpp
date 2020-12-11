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
: systemDriver(driver)
{
	// Create root of system device
	addDeviceType(driver.name, driver.type, 0);

	systemDevice->completeConfig();
}

device_t *SystemConfig::addDeviceType(tag_t *tag, const DeviceType &type, uint64_t clock)
{
	Device *owner = nullptr;

	device_t *sys = type.create(*this, tag, nullptr, clock);

	return addDevice(sys, owner);
}

Device *SystemConfig::addDevice(Device *dev, Device *owner)
{
	const ConfigDeviceStack context(*this);

	if (owner != nullptr) {

	} else {
		// System device
		assert(systemDevice == nullptr);
		systemDevice = dev;
	}

	// Initialize device
	dev->configure(*this);

	return dev;
}

void SystemConfig::begin(Device *dev)
{
	assert(configDevice == nullptr);
	configDevice = dev;
}
