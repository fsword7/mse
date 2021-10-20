/*
 * devproc.h - Processor (CPU) Device package
 *
 *  Created on: Nov 19, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/diexec.h"
#include "emu/dibus.h"
#include "emu/distate.h"
#include "emu/didebug.h"

class cpuDevice
: public Device,
  public diExecute,
  public diExternalBus,
  public diState,
  public diDebug
{
protected:
	cpuDevice(const SystemConfig &config, const DeviceType &type,
			const string &tagName, Device *owner, uint64_t clock);

public:
	virtual ~cpuDevice() = default;

	void setProcessorID(int id)    { cpuid = id; }
	int  getProcessorID() const    { return cpuid; }

private:
	int cpuid = 0;
};

