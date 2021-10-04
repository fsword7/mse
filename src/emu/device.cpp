/*
 * device.cpp - device handling package
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/didebug.h"
#include "emu/scheduler.h"
#include "emu/machine.h"
#include "emu/map/romentry.h"

Device::Device(const SystemConfig &config, const DeviceType &type, cstag_t &name, Device *owner, uint64_t clock)
: sysConfig(config), type(type), devName(name), ownDevice(owner), clock(clock)
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
	configureDevice(config);
}

void Device::start()
{
	devStart();
	startDevice();

	updateClock();

	flagStarted = true;
}

void Device::stop()
{
	devStop();
	stopDevice();
	flagStarted = false;
}

void Device::reset()
{
	devReset();
	resetDevice();
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
	static constexpr romEntry_t firmwareEmpty[] = { ROM_END };

	if (firmwareEntries == nullptr)
	{
		firmwareEntries = devGetROMEntries();
		if (firmwareEntries == nullptr)
			firmwareEntries = getDeviceFirmwareEntries();
		if (firmwareEntries == nullptr)
			firmwareEntries = firmwareEmpty;
	}

	return firmwareEntries;
}

void Device::updateClock()
{
	for (auto &iface : ifList)
		iface->ifUpdateClock();
	updateDeviceClock();
}

Timer *Device::allocateTimer(void *data)
{
	return ownMachine->getScheduler().allocateTimer(*this, 0, data);
}

void Device::setTimer(const attotime_t &duration, void *data, int32_t param)
{
	ownMachine->getScheduler().setTimer(*this, duration, 0, data, param);
}

// ************************************************

DeviceInterface::DeviceInterface(device_t *owner, const tag_t *name)
: owner(owner), tagName(name)
{
	owner->addInterface(this);
}
