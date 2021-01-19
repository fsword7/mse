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

	Machine *findSystem(cstag_t name);
	Device  *findDevice(Console *user, cstag_t name);
	Device  *findDevice(cstag_t name);

	uint64_t getValue(cstag_t sValue);

	CommandStatus create(Console *user, args_t &args);
	CommandStatus dump(Console *user, args_t &args);
	CommandStatus list(Console *user, args_t &args);
	CommandStatus load(Console *user, args_t &args);
	CommandStatus reset(Console *user, args_t &args);
	CommandStatus set(Console *user, args_t &args);
	CommandStatus show(Console *user, args_t &args);
	CommandStatus showDevices(Console *user, args_t &args);
	CommandStatus step(Console *user, args_t &args);
	CommandStatus start(Console *user, args_t &args);

private:
	// Machines
	static vector<Machine *> machines;
};
