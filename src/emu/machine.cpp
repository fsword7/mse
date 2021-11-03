/*
 * machine.cpp
 *
 *  Created on: Oct 11, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/dibus.h"
#include "emu/map/map.h"
#include "emu/romloader.h"
#include "emu/debugger/debugger.h"
#include "emu/engine.h"
#include "emu/video.h"
#include "emu/machine.h"

Machine::Machine(const SystemEngine &engine, const SystemConfig &config, cstag_t &tagName)
: config(config), devName(tagName), busManager(this), scheduler(*this), video(*this)
{
	system = dynamic_cast<sysDevice *>(config.getSystemDevice());

	// Assign machine handler to all devices
	for (Device &dev : DeviceIterator(*system))
		dev.setMachine(this);

	// Assign system bus management to system device
	system->setExternalBusManager(&busManager);
}

Machine::~Machine()
{
	// Close all logging files
	logFile.close(-1);
}

Machine *Machine::create(ostream &out, const SystemEngine &engine, const SystemDriver *driver, cstag_t &tagName)
{
	SystemConfig *config = nullptr;
	Machine *machine = nullptr;

	config  = new SystemConfig(*driver, tagName);
	machine = new Machine(engine, *config, tagName);

	return machine;
}

void Machine::startAllDevices(Console *cty)
{
	for (Device &dev : DeviceIterator(*system))
	{
		cty->printf("%s: starting %s device\n", dev.getDeviceName(), dev.getShortName());
		dev.start();
	}
}

void Machine::reset(Console *cty)
{
}

// Final system configuration initialization
void Machine::start(Console *cty)
{
	assert(system != nullptr);

	// Assign machine handler to all devices
//	for (Device &dev : DeviceIterator(*sysDevice))
//		dev.setMachine(this);
//	for (Device &dev : DeviceIterator(*sysDevice))
//		dev.completeConfig();

	// Iniitalize ROM entries
	loader = new romLoader(this, *cty);

	busManager.init(cty);

	// Finding required objects to being linked
	for (Device &dev : DeviceIterator(*system))
		dev.resolvePostMapping();

	// if (debugFlags & DBGFLG_ENABLED)
	// 	initDebugger();

	// Now start all devices
	startAllDevices(cty);
}

void Machine::stop(Console *cty)
{
	for (Device &dev : DeviceIterator(*system))
	{
		cty->printf("%s: stopping %s device\n", dev.getDeviceName(), dev.getShortName());
		dev.stop();
	}
}

// Launching system in separated threading process
void Machine::launch()
{
	// logFile.log(LOG_CONSOLE, "Started system at thread ID %llX\n",
	// 	std::this_thread::get_id());
	// logFile.flushAll();

	running = true;

	try
	{
		scheduler.init();
		
		while (running)
		{
			// execute CPU processors intervally.
			scheduler.timeslice();
		}
	}

	catch (...)
	{
		logFile.log(LOG_CONSOLE, "Caught unhandled exception\n");
	}

	// stopping system

// 	logFile.log(LOG_CONSOLE, "Halted system at thread ID %X\n",
// 		std::this_thread::get_id());
}

void Machine::halt(Console *user)
{
	// user->printf("Halting thread process ID %X\n", thisThread.get_id());

	flags |= SYSTEM_HALT;
	running = false;

	if (thisThread.joinable())
		thisThread.join();
}

// main loop routine at separating thread.
void Machine::run(Console *user)
{
	
	logFile.setConsole(user);

	// Starting separating process
	thisThread = thread(&Machine::launch, this);
	// user->printf("Started thread process ID %X\n", thisThread.get_id());

	// thisThread.detach();
}