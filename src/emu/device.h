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

	// Getter function calls
	inline tag_t getFullName() const  { return fullName; }
	inline tag_t getShortName() const { return shortName; }

private:
	tag_t fullName = nullptr;
	tag_t shortName = nullptr;

};

typedef Device device_t;
