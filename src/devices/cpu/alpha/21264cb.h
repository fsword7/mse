/*
 * 21264.h - 21264CB Alpha EV68 Processor
 *
 *  Created on: Nov 22, 2020
 *      Author: Tim Stark
 */

#pragma once

#define CPU_TYPE_EV68

#include "devices/cpu/alpha/axp.h"

class axp21264cb_device : public AlphaProcessor
{
public:
	axp21264cb_device(const SystemConfig &config, cstag_t &tagName, Device *owner, uint64_t clock);
	~axp21264cb_device() = default;

	void run();
};

DECLARE_DEVICE_TYPE(axp21264cb, axp21264cb_device)
