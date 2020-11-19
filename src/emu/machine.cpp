/*
 * machine.cpp
 *
 *  Created on: Oct 11, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/machine.h"

Machine::Machine(const SystemConfig &config)
: config(config)
{

}

Machine::~Machine()
{
}

Machine *Machine::create(ostream &out, const SystemDriver *driver, const string devName)
{
	SystemConfig *config = nullptr;
	Machine *sys = nullptr;

	config = new SystemConfig(*driver);
	sys = new Machine(*config);

	return nullptr;
}
