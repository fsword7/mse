/*
 * devsys.h - system device package
 *
 *  Created on: Jul 14, 2020
 *      Author: Tim Stark
 */

#pragma once

class SystemDevice : public Device
{
public:
	SystemDevice(const SystemConfig &config, const DeviceType &type);

};
