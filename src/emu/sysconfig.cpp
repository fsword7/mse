/*
 * sysconfig.cpp - system configuration package
 *
 *  Created on: Jul 14, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/driver.h"
#include "emu/sysconfig.h"

SystemConfig::SystemConfig(const SystemDriver &driver, cstag_t &tagName)
: systemDriver(driver)
{
	// Create root of system device
	addDeviceType(tagName, driver.type, 0);

	// Complete final configuration
	systemDevice->completeConfig();
//	fmt::printf("%s: Total %d devices\n", systemDevice->getDeviceName(), systemDevice->getList().size());
	for (auto *dev : systemDevice->getList()) {
		fmt::printf("%s: Completing final configuration\n", dev->getDeviceName());
		dev->completeConfig();
	}
}

Device *SystemConfig::addDeviceType(cstag_t &tagName, const DeviceType &type, uint64_t clock)
{
	Device *owner = nullptr;

	fmt::printf("%s: Creating %s system...\n", tagName, type.getShortName());

	device_t *sys = type.create(*this, tagName, nullptr, clock);

	return addDevice(sys, owner);
}

Device *SystemConfig::addDevice(Device *dev, Device *owner)
{
	const ConfigDeviceStack context(*this);

	if (owner != nullptr) {
//		assert(configDevice != nullptr);
		fmt::printf("%s: Owner %s(%s) device\n", dev->getDeviceName(), owner->getDeviceName(), owner->getShortName());

		owner->addNode(dev);
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
	fmt::printf("%s: Start device configuration\n", dev->getDeviceName());
}
