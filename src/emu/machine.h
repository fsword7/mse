/*
 * machine.h
 *
 *  Created on: Oct 11, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/map/map.h"
#include "emu/sysconfig.h"
#include "emu/devsys.h"

class Machine
{
public:
	Machine(const SystemConfig &config, cstag_t &tagName);
	~Machine();

	inline cstag_t  &getDeviceName() const   { return devName; }
	inline device_t *getSystemDevice() const { return sysDevice; }
	inline LogFile  *getLogFile() const      { return &logFile; }

	inline aspace::BusManager &getExternalBusManager() { return busManager; }

	static Machine *create(ostream &out, const SystemDriver *driver, cstag_t &devName);

	// Command function calls
	void reset(Console *cty);
	void start(Console *cty);
	void stop(Console *cty);

private:
	const SystemConfig &config;
	SystemDevice *sysDevice = nullptr;

	cstag_t devName;

	mutable LogFile logFile;

	// External bus manager
	aspace::BusManager busManager;
};
