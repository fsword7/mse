/*
 * machine.cpp
 *
 *  Created on: Oct 11, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/dibus.h"
#include "emu/map/map.h"
#include "emu/machine.h"

Machine::Machine(const SystemConfig &config, cstag_t &tagName)
: config(config), devName(tagName), busManager(this)
{
	sysDevice = dynamic_cast<SystemDevice *>(config.getSystemDevice());

	// Assign machine handler to all devices
	for (Device &dev : DeviceIterator(*sysDevice))
		dev.setMachine(this);

	// Assign system bus management to system device
	sysDevice->setExternalBusManager(&busManager);
}

Machine::~Machine()
{
	// Close all logging files
	logFile.close(-1);
}

Machine *Machine::create(ostream &out, const SystemDriver *driver, cstag_t &tagName)
{
	SystemConfig *config = nullptr;
	Machine *machine = nullptr;

	config  = new SystemConfig(*driver, tagName);
	machine = new Machine(*config, tagName);

	return machine;
}

void Machine::reset(Console *cty)
{
}

// Final system configuration initialization
void Machine::start(Console *cty)
{
	assert(sysDevice != nullptr);

	// Assign machine handler to all devices
//	for (Device &dev : DeviceIterator(*sysDevice))
//		dev.setMachine(this);
//	for (Device &dev : DeviceIterator(*sysDevice))
//		dev.completeConfig();

	busManager.init(cty);
}

void Machine::stop(Console *cty)
{
}
