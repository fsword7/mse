/*
 * devproc.h - Processor (CPU) Device package
 *
 *  Created on: Nov 19, 2020
 *      Author: Tim Stark
 */

#pragma once

class ProcessorDevice : public Device
{
public:
	ProcessorDevice(const SystemConfig &config, const DeviceType &type);

};
