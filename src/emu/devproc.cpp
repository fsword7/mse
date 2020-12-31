/*
 * devproc.cpp - Processor (CPU) Device package
 *
 *  Created on: Nov 19, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/devproc.h"

ProcessorDevice::ProcessorDevice(const SystemConfig &config, const DeviceType &type,
		const string &tagName, Device *owner, uint64_t clock, endian_t eType,
		int dWidth, int dRadix, int aWidth, int aRadix, int aShift)
: Device(config, type, tagName, owner, clock),
  diExecute(this),
  diMemory(this),
  diDebug(this)
{

}
