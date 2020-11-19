/*
 * sysconfig.h - system configuration package
 *
 *  Created on: Jul 14, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/driver.h"
#include "emu/device.h"

class SystemConfig
{
public:
	SystemConfig(const SystemDriver &driver);

	device_t *addDeviceType(tag_t *tag, const DeviceType &type, uint64_t clock);

private:
	const SystemDriver &sysDriver;
};
