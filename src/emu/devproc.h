/*
 * devproc.h - Processor (CPU) Device package
 *
 *  Created on: Nov 19, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/diexec.h"
#include "emu/dimem.h"
#include "emu/didebug.h"

class ProcessorDevice
: public Device,
  public diExecute,
  public diMemory,
  public diDebug
{
public:

protected:
	ProcessorDevice(const SystemConfig &config, const DeviceType &type,
			const string &tagName, Device *owner, uint64_t clock, endian_t eType,
			int dWidth, int dRadix, int aWidth, int aRadix, int aShift);

public:
	virtual ~ProcessorDevice() = default;

};
