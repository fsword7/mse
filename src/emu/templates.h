/*
 * templates.h
 *
 *  Created on: Dec 12, 2020
 *      Author: Tim Stark
 */

#pragma once

template <class DeviceClass> template <typename... Args>
inline DeviceClass *DeviceCreator<DeviceClass>::operator ()(SystemConfig &config, cstag_t &devName, Args&&... args) const
{
	return dynamic_cast<DeviceClass *>(config.addDeviceType(devName, *this, forward<Args>(args)...));
}

template <class DeviceClass> template <typename Exposed, bool Required, typename ... Args>
DeviceClass &DeviceCreator<DeviceClass>::operator ()(SystemConfig &config, DeviceFinder<Exposed, Required> &finder, cstag_t &devName, Args &&... args) const
{
	// assert(&config.getCurrentDevice() == &finder.getOwnerDevice());
	DeviceClass &device(dynamic_cast<DeviceClass &>(*config.addDeviceType(devName, *this, std::forward<Args>(args)...)));

	finder = device;
	return device;
}
