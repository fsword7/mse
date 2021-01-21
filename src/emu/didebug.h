/*
 * didebug.h - Device Interface - Debugging tools
 *
 *  Created on: Nov 27, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/map/map.h"
#include "emu/debug.h"

class diDebug : public DeviceInterface
{
public:
	diDebug(device_t *owner);
	virtual ~diDebug() = default;

	virtual int list(Console *cty, offs_t vAddr) = 0;

#ifdef DEBUG
protected:
	Debug dbg;
#endif /* DEBUG */

private:
};
