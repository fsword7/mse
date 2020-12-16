/*
 * dibus.h - bus interface package
 *
 *  Created on: Dec 16, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/map/map.h"

class diBus : public DeviceInterface
{
public:
	diBus(device_t *owner);
	~diBus() = default;

private:
	vector<const mapAddressConfig *> mapConfig;
	vector<mapAddressSpace *> mapSpace;
};
