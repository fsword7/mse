/*
 * ev6.cpp - DEC 21264 Alpha processor series (EV6 series)
 *
 *  Created on: Dec 11, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/alpha/ev6.h"

DEFINE_DEVICE_TYPE(dec21264, dec21264_cpuDevice, "21264", "DEC 21264 Alpha Processor")

dec21264_cpuDevice::dec21264_cpuDevice(const SystemConfig &config,
		cstag_t &tagName, Device *owner, uint64_t clock)
: AlphaProcessor(config, dec21264, tagName, owner, clock, 44)
{
	setArchType(ARCH_EV6);
	setArchFlags(ARCH_PFM|ARCH_PRC|ARCH_MVI|ARCH_CIX|ARCH_BWX);
}
