/*
 * templates.h
 *
 *  Created on: Dec 12, 2020
 *      Author: Tim Stark
 */

#pragma once

template <class DeviceClass> template <typename... Args>
inline DeviceClass *DeviceCreator<DeviceClass>::operator ()(SystemConfig &config, cstag_t &tagName, Args&&... args) const
{
	return dynamic_cast<DeviceClass *>(config.addDeviceType(tagName, *this, forward<Args>(args)...));
//	return nullptr;
}
