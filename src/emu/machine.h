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

private:
	const SystemConfig &config;
};
