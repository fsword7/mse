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


CommandStatus SystemEngine::create(ostream &out, args_t args)
{
	SystemList list;
	string devName = args.getNext();
	string sysName = args.getNext();
	const SystemDriver *driver = nullptr;
	Machine *sys = nullptr;

	if (find(devName) != nullptr) {
		fmt::fprintf(out, "%s: system already created.\n", devName);
		return CommandStatus::cmdOk;
	}

	driver = list.find(sysName);
	if (driver == nullptr) {
		fmt::fprintf(out, "%s: system '%s' not recognized.\n", devName, sysName);
		return CommandStatus::cmdOk;
	}

	sys = Machine::create(out, driver, devName);
	if (sys == nullptr)
	{
		fmt::fprintf(out, "%s: system creation failure for %s - aborted.\n", devName, sysName);
		return CommandStatus::cmdOk;
	}

	// Add new system to machines list
	fmt::fprintf(out, "%s: system %s created successfully\n", devName, sysName);
	machines.push_back(sys);
	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::dump(ostream &out, args_t args)
{
	string devName = args.getNext();

	Machine *sys = find(devName);
	if (sys == nullptr)
	{
		fmt::fprintf(out, "%s: system not found\n", devName);
		return CommandStatus::cmdOk;
	}

	Device *dev = sys->getSystemDevice();
	if (!dev->hasBusInterface())
	{
		fmt::fprintf(out, "%s: do not have bus interface\n", devName);
		return CommandStatus::cmdOk;
	}


	return CommandStatus::cmdOk;
}
