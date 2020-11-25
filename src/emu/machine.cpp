/*
 * machine.cpp
 *
 *  Created on: Oct 11, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/machine.h"

Machine::Machine(const SystemConfig &config, tag_t *devName)
: config(config), devName(strdup(devName))
{
}

Machine::~Machine()
{
	if (devName != nullptr)
		delete devName;
}

Machine *Machine::create(ostream &out, const SystemDriver *driver, const string devName)
{
	SystemConfig *config = nullptr;
	Machine *machine = nullptr;

	config  = new SystemConfig(*driver);
	machine = new Machine(*config, devName.c_str());

	return machine;
}
