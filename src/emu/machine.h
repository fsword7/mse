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

class romLoader;

class Machine
{
public:
	Machine(const SystemConfig &config, cstag_t &tagName);
	~Machine();

	inline cstag_t  &getDeviceName() const   { return devName; }
	inline device_t *getSystemDevice() const { return system; }
	inline LogFile  *getLogFile() const      { return &logFile; }

	inline void setConsole(Console *user)	 { logFile.setConsole(user); }
	inline void setDialedDevice(Device *dev) { dialedDevice = dev; }
	inline Device *getDialedDevice() const   { return dialedDevice; }

	inline aspace::BusManager &getExternalBusManager() { return busManager; }

	static Machine *create(ostream &out, const SystemDriver *driver, cstag_t &devName);

	// Command function calls
	void reset(Console *cty);
	void start(Console *cty);
	void stop(Console *cty);

private:
	const SystemConfig &config;
	sysDevice *system = nullptr;
	mutable Device *dialedDevice = nullptr;

	cstag_t devName;

	mutable fs::path ownFolder;
	mutable LogFile  logFile;

	romLoader *loader = nullptr;

	// External bus manager
	aspace::BusManager busManager;
};
