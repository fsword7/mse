/*
 * device.cpp - debugger package for CPU processors
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/debugger/symbols.h"
#include "emu/debugger/device.h"

using namespace emu::debug;

DeviceDebugger::DeviceDebugger(Device *device)
: symTable(device->getMachine(), device)
{

}

DeviceDebugger::~DeviceDebugger()
{

}