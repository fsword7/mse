/*
 * ev6.h - DEC 21264 Alpha Processor series
 *
 *  Created on: Nov 22, 2020
 *      Author: Tim Stark
 */

#pragma once

#define CPUID_21264CB // 21264 EV68

#include "dev/cpu/alpha/axp.h"

class dec21264_cpuDevice : public alpha_cpuDevice
{
public:
	dec21264_cpuDevice(const SystemConfig &config, cstag_t &tagName, Device *owner, uint64_t clock);
	~dec21264_cpuDevice() = default;

	void preset() override; // individual processor reset
	int abort(int why); // Aborting with internal exception codes
	void enterException(int excCode) override;

	int translate(uint64_t vAddr, uint64_t &pAddr, bool &asmb, int accFlags);

	// Virtual PAL hardware instruction calls
	void call_pal(uint32_t opWord) override; // PAL00 instruction
	void hw_mfpr(uint32_t opWord) override;  // PAL19 instruction
	void hw_mtpr(uint32_t opWord) override;  // PAL1D instruction
	void hw_ld(uint32_t opWord) override;    // PAL1B instruction
	void hw_st(uint32_t opWord) override;    // PAL1F instruction
	void hw_ret(uint32_t opWord) override;   // PAL1E instruction

//	void run();

private:
	// Internal processor registers
	struct ev6state
	{
		iCtl_t ictl;
	} ev6;
};

DECLARE_DEVICE_TYPE(dec21264, dec21264_cpuDevice)
