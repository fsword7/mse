/*
 * devproc.cpp - Processor (CPU) Device package
 *
 *  Created on: Nov 19, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/devproc.h"

ProcessorDevice::ProcessorDevice(const SystemConfig &config, const DeviceType &type)
: Device(config, type),
  diExecute(this),
  diMemory(this),
  diDebug(this)
{

}
