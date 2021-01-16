/*
 * devsys.h - system device package
 *
 *  Created on: Jul 14, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/map/map.h"

class SystemDevice : public Device
{
public:
	SystemDevice(const SystemConfig &config, const DeviceType &type, cstag_t &tagName, uint64_t clock);

	inline void setExternalBusManager(aspace::BusManager *bus) { busManager = bus; }

	// Virtual device function calls
	void devConfigure(SystemConfig &config);

private:
	const SystemDriver &driver;

protected:
	aspace::BusManager *busManager = nullptr; // system-wide bus manager for all devices
};
