/*
 * engine.h - System engine package
 *
 *  Created on: Jul 31, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/command.h"
#include "emu/machine.h"

class SystemEngine
{
public:
	SystemEngine() = default;
	~SystemEngine() = default;

	// Global system initialization
	void ginit();
	void gexit();

	Machine *find(const string sysName);

	CommandStatus create(Console *cty, args_t args);
	CommandStatus dump(Console *cty, args_t args);
	CommandStatus showDevices(Console *cty, args_t args);

private:
	// Machines
	static vector<Machine *> machines;
};
