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
#include "emu/machine.h"

Machine::Machine(const SystemConfig &config, cstag_t &tagName)
: config(config), devName(tagName), busManager(this)
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
	assert(system != nullptr);

	// Assign machine handler to all devices
//	for (Device &dev : DeviceIterator(*sysDevice))
//		dev.setMachine(this);
//	for (Device &dev : DeviceIterator(*sysDevice))
//		dev.completeConfig();

	// Iniitalize ROM entries
	loader = new romLoader(this, *cty);

	busManager.init(cty);
}

void Machine::stop(Console *cty)
{
}
