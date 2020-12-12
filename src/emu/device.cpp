/*
 * device.cpp - device handling package
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"

Device::Device(const SystemConfig &config, const DeviceType &type, cstag_t &name, Device *owner, uint64_t clock)
: sysConfig(config), type(type), devName(name)
{
	ifList.clear();
}

void Device::configure(SystemConfig &config)
{
	assert(&config == &sysConfig);

	// device initialization
	config.begin(this);
	devConfigure(config);
}

// Complete final device configuration
void Device::completeConfig()
{

	for (auto *iface : ifList)
	{
		fmt::printf("%s: completeing %s interface...\n", getShortName(), iface->getName());
		iface->completeConfig();
	}
}

DeviceInterface::DeviceInterface(device_t *owner, const tag_t *name)
: owner(owner), tagName(name)
{
	Device::ifList_t list = owner->getInterfaces();

	if (list.size() > 0)
		list.back()->next = this;
	list.push_back(this);
}
