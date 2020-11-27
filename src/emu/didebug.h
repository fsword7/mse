/*
 * didebug.h - Device Interface - Debugging tools
 *
 *  Created on: Nov 27, 2020
 *      Author: Tim Stark
 */

#pragma once

class diDebug : public DeviceInterface
{
public:
	diDebug(device_t *owner);
	~diDebug() = default;

private:
};
