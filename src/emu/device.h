/*
 * device.h - device handling package
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/delegate.h"

class Device : public Delegate
{
public:
	Device() = default;
	~Device() = default;
};
