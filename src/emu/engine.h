/*
 * engine.h - System engine package
 *
 *  Created on: Jul 31, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/command.h"

class SystemEngine
{
public:
	SystemEngine() = default;
	~SystemEngine() = default;

	CommandStatus create(ostream &out, int argc, args_t args);

private:
	// Machines
};
