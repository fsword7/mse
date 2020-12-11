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

	inline Device *getSystemDevice() const { return systemDevice; }
	inline Device *getConfigDevice() const { return configDevice; }

	Device *addDeviceType(tag_t *tag, const DeviceType &type, uint64_t clock);
	Device *addDevice(Device *dev, Device *owner);

	const SystemDriver &getSystemDriver() const { return systemDriver; }

	void begin(Device *dev);

protected:
	class ConfigDeviceStack
	{
	public:
		ConfigDeviceStack(SystemConfig &config)
		: host(config), device(config.configDevice)
		{
			host.configDevice = nullptr;
		}

		~ConfigDeviceStack()
		{
			host.configDevice = device;
		}

	private:
		SystemConfig &host;
		Device *device = nullptr;
	};

private:
	const SystemDriver &systemDriver;

	Device *systemDevice = nullptr;
	Device *configDevice = nullptr;
};
