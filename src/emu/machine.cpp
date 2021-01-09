/*
 * machine.cpp
 *
 *  Created on: Oct 11, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/machine.h"

Machine::Machine(const SystemConfig &config, cstag_t &tagName)
: config(config), devName(tagName), busManager(this)
{
	sysDevice = dynamic_cast<SystemDevice *>(config.getSystemDevice());

	// Assign machine handler to all devices
	for (Device &dev : DeviceIterator(*sysDevice))
		dev.setMachine(this);
}

Machine::~Machine()
{
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

void Machine::start(Console *cty)
{
	assert(sysDevice != nullptr);
	busManager.init(cty);
}

void Machine::stop(Console *cty)
{
}
