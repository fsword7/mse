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

	void setLogFile(Machine *sys); // { dbg.setLogFile(sys->getLogFile()); }

	virtual int list(Console *cty, offs_t vAddr);

#ifdef ENABLE_DEBUG
	inline Debug *getDebugSetting() const { return &dbg; }
#endif /* ENABLE_DEBUG */

#ifdef ENABLE_DEBUG
protected:
	mutable Debug dbg;
#endif /* ENABLE_DEBUG */

private:
};
