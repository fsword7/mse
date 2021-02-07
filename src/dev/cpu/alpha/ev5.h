/*
 * ev5.h - 21164 series - EV5 architecture
 *
 *  Created on: Feb 6, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "dev/cpu/alpha/axp.h"

#include "dev/cpu/alpha/ev5_ipr.h"

#define CPUID_AXP21164

class ev5_device : public AlphaProcessor
{
public:
	ev5_device(const SystemConfig &config, cstag_t &tagName, Device *owner, uint64_t clock);
	~ev5_device() = default;

	// Virtual PAL hardware instruction calls
	void hw_mtpr(uint32_t opWord) override;
	void hw_mfpr(uint32_t opWord) override;
};

DECLARE_DEVICE_TYPE(ev5, ev5_device)
