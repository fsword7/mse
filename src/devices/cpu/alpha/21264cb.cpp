/*
 * 21264cb.cpp
 *
 *  Created on: Dec 11, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "devices/cpu/alpha/21264cb.h"

DEFINE_DEVICE_TYPE(axp21264cb, axp21264cb_device, "21264CB", "AXP 21264CB Processor")

axp21264cb_device::axp21264cb_device(const SystemConfig &config, const DeviceType &type) // tag_t *tag, device_t *owner, uint64_t clock)
: AlphaProcessor(config, axp21264cb) // , tag, owner, clock)
{
}

axp21264cb_device::~axp21264cb_device()
{
}

void axp21264cb_device::run()
{
}
