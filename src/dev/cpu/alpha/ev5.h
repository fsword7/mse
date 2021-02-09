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

class dec21164_cpuDevice : public AlphaProcessor
{
public:
	dec21164_cpuDevice(const SystemConfig &config, cstag_t &tagName, Device *owner, uint64_t clock);
	~dec21164_cpuDevice() = default;

	// Virtual PAL hardware instruction calls
	void call_pal(uint32_t opWord) override; // PAL00 instruction
	void hw_mfpr(uint32_t opWord) override;  // PAL19 instruction
	void hw_mtpr(uint32_t opWord) override;  // PAL1D instruction
	void hw_ld(uint32_t opWord) override;    // PAL1B instruction
	void hw_st(uint32_t opWord) override;    // PAL1F instruction
	void hw_ret(uint32_t opWord) override;   // PAL1E instruction

};

DECLARE_DEVICE_TYPE(dec21164, dec21164_cpuDevice);
