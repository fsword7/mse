/*
 * diexec.h - Device Interface - Execution
 *
 *  Created on: Nov 27, 2020
 *      Author: Tim Stark
 */

#pragma once

class diExecute : public DeviceInterface
{
public:
	diExecute(device_t *owner);
	~diExecute() = default;

	// Abstract function calls
	virtual void run() = 0;

private:
};
