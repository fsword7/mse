/*
 * ev6.h - DEC 21264 Alpha Processor series
 *
 *  Created on: Nov 22, 2020
 *      Author: Tim Stark
 */

#pragma once

#define CPUID_21264CB // 21264 EV68

#include "dev/cpu/alpha/axp.h"

class dec21264_cpuDevice : public AlphaProcessor
{
public:
	dec21264_cpuDevice(const SystemConfig &config, cstag_t &tagName, Device *owner, uint64_t clock);
	~dec21264_cpuDevice() = default;

	// Virtual PAL hardware instruction calls
	void hw_mfpr(uint32_t opWord) override;  // PAL19 instruction
	void hw_mtpr(uint32_t opWord) override;  // PAL1D instruction
//	void hw_ld(uint32_t opWord) override;    // PAL1B instruction
//	void hw_st(uint32_t opWord) override;    // PAL1F instruction
//	void hw_ret(uint32_t opWord) override;   // PAL1E instruction

//	void run();
};

DECLARE_DEVICE_TYPE(dec21264, dec21264_cpuDevice)
