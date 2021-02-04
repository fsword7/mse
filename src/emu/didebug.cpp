/*
 * didebug.cpp - Device Interface - Debugging Tools
 *
 *  Created on: Nov 27, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/didebug.h"
#include "emu/machine.h"

diDebug::diDebug(device_t *owner)
: DeviceInterface(owner, "debug")
{
	owner->ifDebug = this;
}

//void diDebug::ifStart()
//{
//	Machine *sys = getDevice()->getMachine();
//	assert(sys != nullptr);
//
//#ifdef DEBUG
//	dbg.setLogFile(sys->getLogFile());
//#endif /* DEBUG */
//}

void diDebug::setLogFile(Machine *sys)
{
	assert(sys != nullptr);
	dbg.setLogFile(sys->getLogFile());
}
