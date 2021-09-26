/*
 * machine.h
 *
 *  Created on: Oct 11, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/map/map.h"
#include "emu/sysconfig.h"
#include "emu/scheduler.h"
#include "emu/devsys.h"

class romLoader;

// Debug flags definition
constexpr uint32_t DBGFLG_ENABLED =	0x00000001;

class SystemEngine;

class Machine
{
public:
	Machine(const SystemEngine &engine, const SystemConfig &config, cstag_t &tagName);
	~Machine();

	inline cstag_t  &getDeviceName() const   { return devName; }
	inline device_t *getSystemDevice() const { return system; }
	inline LogFile  *getLogFile() const      { return &logFile; }
	inline DeviceScheduler &getScheduler()   { return scheduler; }

	inline void setConsole(Console *user)	 { logFile.setConsole(user); }
	inline void setDialedDevice(Device *dev) { dialedDevice = dev; }
	inline Device *getDialedDevice() const   { return dialedDevice; }

	inline aspace::BusManager &getExternalBusManager() { return busManager; }

	inline bool isDebuggerEnabled() { return debugFlags & DBGFLG_ENABLED; }

	static Machine *create(ostream &out, const SystemEngine &engine, const SystemDriver *driver, cstag_t &devName);

	void startAllDevices(Console *cty);

	// Command function calls
	void reset(Console *cty);
	void start(Console *cty);
	void stop(Console *cty);
	void run(Console *cty);

private:
	const SystemConfig &config;
	sysDevice *system = nullptr;

	uint32_t debugFlags = 0;

	mutable Device *dialedDevice = nullptr;

	cstag_t devName;

	mutable fs::path ownFolder;
	mutable LogFile  logFile;

	romLoader *loader = nullptr;

	// External bus manager
	aspace::BusManager busManager;
	DeviceScheduler scheduler;
};
