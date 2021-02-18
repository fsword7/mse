/*
 * flash.cpp - Flash memory device package
 *
 *  Created on: Feb 17, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/chip/flash.h"

flash_memDevice::flash_memDevice(const SystemConfig &config, const DeviceType &type,
	cstag_t &tagName, Device *owner, uint64_t clock)
: Device(config, type, tagName, owner, clock),
  nvmInterface(this, "flash")
{

}

flash_memDevice::~flash_memDevice()
{
	// Save contents
}
