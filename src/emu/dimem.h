/*
 * dimem.h - Device Interface - Memory
 *
 *  Created on: Nov 27, 2020
 *      Author: Tim Stark
 */

#pragma once

class diMemory : public DeviceInterface
{
public:
	diMemory(device_t *owner);
	~diMemory() = default;

private:
};
