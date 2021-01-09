/*
 * engine.cpp - System engine handler package
 *
 *  Created on: Jul 31, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/command.h"
#include "emu/engine.h"
#include "emu/syslist.h"

vector<Machine *> SystemEngine::machines;

// Global system initialization routines
void SystemEngine::ginit()
{
	machines.clear();
}

// Global system shutdown and cleanup routines
void SystemEngine::gexit()
{
	// Release all machines from memory allocation
	for (auto mach : machines)
		delete mach;
	machines.clear();
}


Machine *SystemEngine::find(const string sysName)
{
	for (auto mach : machines)
		if (sysName == mach->getDeviceName())
			return mach;
	return nullptr;
}


CommandStatus SystemEngine::create(Console *cty, args_t args)
{
	SystemList list;
	string devName = args.getNext();
	string sysName = args.getNext();
	const SystemDriver *driver = nullptr;
	Machine *sys = nullptr;

	if (find(devName) != nullptr) {
		fmt::fprintf(cout, "%s: system already created.\n", devName);
		return CommandStatus::cmdOk;
	}

	driver = list.find(sysName);
	if (driver == nullptr) {
		fmt::fprintf(cout, "%s: system '%s' not recognized.\n", devName, sysName);
		return CommandStatus::cmdOk;
	}

	sys = Machine::create(cout, driver, devName);
	if (sys == nullptr)
	{
		fmt::fprintf(cout, "%s: system creation failure for %s - aborted.\n", devName, sysName);
		return CommandStatus::cmdOk;
	}

	// Add new system to machines list
	fmt::fprintf(cout, "%s: system %s created successfully\n", devName, sysName);
	machines.push_back(sys);

	// Dial system as default
	cty->setDialedSystem(sys->getSystemDevice());
	cty->setDialedDevice(nullptr);

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::dump(Console *cty, args_t args)
{
	string devName = args.getNext();

	Machine *sys = find(devName);
	if (sys == nullptr)
	{
		fmt::fprintf(cout, "%s: system not found\n", devName);
		return CommandStatus::cmdOk;
	}

	Device *dev = sys->getSystemDevice();
	diExternalBus *mem;;
	if (!dev->hasInterface(mem))
	{
		fmt::fprintf(cout, "%s: do not have external bus interface\n", devName);
		return CommandStatus::cmdOk;
	}
//	mapAddressSpace &space = dev->getAddressSpace();


	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::showDevices(Console *cty, args_t args)
{
	Device *sysDevice = cty->getDialedSystem();
	if (sysDevice == nullptr)
	{
		fmt::printf("Please dial system first\n");
		return CommandStatus::cmdOk;
	}

	for (Device &dev : DeviceIterator(*sysDevice))
	{
		fmt::printf("%s\n", dev.getDeviceName());
	}

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::start(Console *cty, args_t args)
{
//	Device *sysDevice = cty->getDialedSystem();
//	if (sysDevice == nullptr)
//	{
//		fmt::printf("Please dial system first\n");
//		return CommandStatus::cmdOk;
//	}
//	Machine *sys = sysDevice->getMachine();

	string devName = args.getNext();
	Machine *sys = find(devName);
	if (sys == nullptr) {
		fmt::fprintf(cout, "%s: system not found.\n", devName);
		return CommandStatus::cmdOk;
	}

	// Start system device as final initialization
	sys->start(cty);

	return CommandStatus::cmdOk;
}
