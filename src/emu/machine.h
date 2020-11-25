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
	Machine(const SystemConfig &config, tag_t *devName);
	~Machine();

	inline tag_t *getDeviceName() const { return devName; }
	inline device_t *getSystemDevice() const { return config.getSystemDevice(); }

	static Machine *create(ostream &out, const SystemDriver *driver, const string devName);

private:
	const SystemConfig &config;

	tag_t *devName;
};
