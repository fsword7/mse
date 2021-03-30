/*
 * device.cpp - device handling package
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/didebug.h"
#include "emu/machine.h"
#include "emu/map/romentry.h"

Device::Device(const SystemConfig &config, const DeviceType &type, cstag_t &name, Device *owner, uint64_t clock)
: sysConfig(config), type(type), devName(name), ownDevice(owner)
{
	ifList.clear();
}

// Local device function calls
void Device::configure(SystemConfig &config)
{
	assert(&config == &sysConfig);

	// device initialization
	config.begin(this);
	devConfigure(config);
}

void Device::start()
{
	devStart();
	flagStarted = true;
}

void Device::stop()
{
	devStop();
	flagStarted = false;
}

void Device::reset()
{
	devReset();
}

void Device::setMachine(Machine *sys)
{
	assert(sys != nullptr);
	ownMachine = sys;

	if (ifDebug != nullptr)
		ifDebug->setLogFile(sys);
}

void Device::addInterface(DeviceInterface *iface)
{
	if (ifList.size() > 0)
		ifList.back()->next = iface;
	ifList.push_back(iface);
}

// Complete final device configuration
void Device::completeConfig()
{
//	fmt::printf("%s: Total %d interfaces\n", getDeviceName(), ifList.size());
//	cout << flush;
	for (auto *iface : ifList)
	{
//		fmt::printf("%s: completeing %s interface...\n", getShortName(), iface->getName());
//		cout << flush;
		iface->completeConfig();
	}
}

cromEntry_t *Device::mapGetROMEntries()
{
	static constexpr romEntry_t romEmpty[] = { ROM_END };

	if (romEntries == nullptr)
	{
		romEntries = devGetROMEntries();
		if (romEntries == nullptr)
			romEntries = romEmpty;
	}

	return romEntries;
}

// ************************************************

DeviceInterface::DeviceInterface(device_t *owner, const tag_t *name)
: owner(owner), tagName(name)
{
	owner->addInterface(this);
}
