/*
 * devproc.h - Processor (CPU) Device package
 *
 *  Created on: Nov 19, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/map/map.h"
#include "emu/diexec.h"
#include "emu/dibus.h"
#include "emu/didebug.h"

class ProcessorDevice
: public Device,
  public diExecute,
  public diExternalBus,
  public diDebug
{
protected:
	ProcessorDevice(const SystemConfig &config, const DeviceType &type,
			const string &tagName, Device *owner, uint64_t clock);

public:
	virtual ~ProcessorDevice() = default;

};
