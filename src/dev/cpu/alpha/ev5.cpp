/*
 * ev5.cpp - 21164 series - main package
 *
 *  Created on: Feb 6, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/alpha/ev5.h"

DEFINE_DEVICE_TYPE(dec21164, ev5_device, "21164", "DEC 21164 AXP Processor")

ev5_device::ev5_device(const SystemConfig &config,
		cstag_t &tagName, Device *owner, uint64_t clock)
: AlphaProcessor(config, dec21164, tagName, owner, clock, 40)
{
}
