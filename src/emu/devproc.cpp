/*
 * devproc.cpp - Processor (CPU) Device package
 *
 *  Created on: Nov 19, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/devproc.h"

ProcessorDevice::ProcessorDevice(const SystemConfig &config, const DeviceType &type,
		const string &tagName, Device *owner, uint64_t clock)
: Device(config, type, tagName, owner, clock),
  diExecute(this),
  diExternalBus(this, "system"),
  diDebug(this)
{

}
