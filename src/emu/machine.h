/*
 * machine.h
 *
 *  Created on: Oct 11, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/sysconfig.h"

class Machine
{
public:
	Machine(const SystemConfig &config);
	~Machine();

	static Machine *create(ostream &out, const SystemDriver *driver, const string devName);

private:
	const SystemConfig &config;
};
