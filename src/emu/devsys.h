/*
 * devsys.h - system device package
 *
 *  Created on: Jul 14, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/map/map.h"
#include "emu/dibus.h"
#include "emu/didebug.h"

class SystemDevice
: public Device,
  public diBus,
  public diDebug
{
public:
	SystemDevice(const SystemConfig &config, const DeviceType &type, cstag_t &tagName, uint64_t clock);

	// Virtual device function calls
	void devConfigure(SystemConfig &config);

private:
	const SystemDriver &driver;

//	mapAddressSpace space; // mapping system bus
	BusManager busManager; // system bus manager for all devices
};
