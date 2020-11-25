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

	inline device_t *getSystemDevice() const { return systemDevice; }

	device_t *addDeviceType(tag_t *tag, const DeviceType &type, uint64_t clock);

private:
	const SystemDriver &sysDriver;

	device_t *systemDevice = nullptr;
};
