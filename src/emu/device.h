/*
 * device.h - device handling package
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/delegate.h"

class device_t : public Delegate
{
public:
	device_t() = default;
	~device_t() = default;

	// Getter function calls
	inline tag_t getFullName() const  { return fullName; }
	inline tag_t getShortName() const { return shortName; }

private:
	tag_t fullName = nullptr;
	tag_t shortName = nullptr;

};
