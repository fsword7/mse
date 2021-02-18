/*
 * flash.h - Flash memory device package
 *
 *  Created on: Feb 17, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "dev/chip/nvmem.h"

class flash_memDevice : public Device, public nvmInterface
{
public:
	flash_memDevice(const SystemConfig &config, const DeviceType &type,
		cstag_t &tagName, Device *owner, uint64_t clock);
	~flash_memDevice();
};
