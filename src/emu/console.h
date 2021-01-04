/*
 * console.h
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/command.h"

class Console
{
public:
	Console() = default;
	~Console() = default;

	void prompt();

	inline void setDialedSystem(Device *device) { dialedSystem = device; }
	inline void setDialedDevice(Device *device) { dialedDevice = device; }

	inline Device *getDialedSystem() const { return dialedSystem; }
	inline Device *getDialedDevice() const { return dialedDevice; }

private:
	Device *dialedSystem = nullptr;
	Device *dialedDevice = nullptr;
};
