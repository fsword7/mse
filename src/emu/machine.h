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
	Machine(const SystemConfig &config, cstag_t &tagName);
	~Machine();

	inline cstag_t  &getDeviceName() const   { return devName; }
	inline device_t *getSystemDevice() const { return config.getSystemDevice(); }

	static Machine *create(ostream &out, const SystemDriver *driver, cstag_t &devName);

private:
	const SystemConfig &config;

	cstag_t devName;
};
