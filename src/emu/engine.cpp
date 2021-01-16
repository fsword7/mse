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


Machine *SystemEngine::findSystem(const string sysName)
{
	for (auto mach : machines)
		if (sysName == mach->getDeviceName())
			return mach;
	return nullptr;
}

Device *SystemEngine::findDevice(Console *cty, cstag_t name)
{
	Device *sysDevice = cty->getDialedSystem();
	if (sysDevice == nullptr)
	{
		fmt::printf("Please dial system first\n");
		return nullptr;
	}

	for (Device &dev : DeviceIterator(*sysDevice))
		if (dev.getDeviceName() == name)
			return &dev;

	return nullptr;
}

Device *SystemEngine::findDevice(cstag_t name)
{
	return nullptr;
}

// K = kilobyte - 1024 bytes
// M = megabyte - 1024*1024 bytes
// G = gigabyte - 1024*1024*1024 bytes
// T = terabyte - 1024*1024*1024*1024 bytes

ctag_t *byteScale = " kmgt";

uint64_t SystemEngine::getValue(cstag_t sValue)
{
	uint64_t value = 0;
	char     bType = 0;

	stringstream ssValue(sValue);

	ssValue >> value;
	ssValue >> bType;

//	fmt::printf("Value: %lld Scale: %c\n", value, bType);
	if (bType != 0) {
		uint64_t scale = 1;
		for (int idx = 0; idx < strlen(byteScale); idx++) {
//			fmt::printf("Scale: %c  Value: %lld\n", byteScale[idx], scale);
			if (bType == byteScale[idx]) {
				value *= scale;
				break;
			}
			scale *= 1024;
		}
	}
//	fmt::printf("Value: %lld (%llX) bytes\n", value, value);

	return value;
}

CommandStatus SystemEngine::create(Console *cty, args_t args)
{
	SystemList list;
	string devName = args.getNext();
	string sysName = args.getNext();
	const SystemDriver *driver = nullptr;
	Machine *sys = nullptr;

	if (findSystem(devName) != nullptr) {
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

	Machine *sys = findSystem(devName);
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

CommandStatus SystemEngine::set(Console *cty, args_t args)
{
	Device *dev = findDevice(cty, args.current());
	if (dev == nullptr) {
		fmt::printf("%s: unknown device\n", args.current());
		return CommandStatus::cmdOk;
	}

	devCommand_t *cmdList = dev->getCommands();
	if (cmdList == nullptr) {
		fmt::printf("%s: do not handle device commands\n", dev->getDeviceName());
		return CommandStatus::cmdOk;
	}

	args.next();
	for (int idx = 0; cmdList[idx].name; idx++) {
		if (cmdList[idx].name == args.current()) {
			args.next();
			if (cmdList[idx].func != nullptr)
				return cmdList[idx].func(cty, dev, args);
		}
	}

	fmt::printf("%s: Unknown device command\n", dev->getDeviceName());

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::show(Console *cty, args_t args)
{
	Device *dev = findDevice(cty, args.current());
	if (dev == nullptr) {
		fmt::printf("%s: unknown device\n", args.current());
		return CommandStatus::cmdOk;
	}

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::showDevices(Console *cty, args_t args)
{
	Device *root = findDevice(cty, args.current());
	if (root == nullptr) {
		fmt::printf("No such devices\n");
		return CommandStatus::cmdOk;
	}

	for (Device &dev : DeviceIterator(*root))
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
	Machine *sys = findSystem(devName);
	if (sys == nullptr) {
		fmt::fprintf(cout, "%s: system not found.\n", devName);
		return CommandStatus::cmdOk;
	}

	// Start system device as final initialization
	sys->start(cty);

	return CommandStatus::cmdOk;
}
