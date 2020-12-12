/*
 * machine.cpp
 *
 *  Created on: Oct 11, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/machine.h"

Machine::Machine(const SystemConfig &config, cstag_t &tagName)
: config(config), devName(tagName)
{
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
