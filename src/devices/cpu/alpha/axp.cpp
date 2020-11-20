/*
 * alpha.cpp - DEC Alpha CPU Processor package
 *
 *  Created on: Nov 20, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/device.h"
#include "emu/sysconfig.h"
#include "devices/cpu/alpha/axp.h"

AlphaProcessor::AlphaProcessor(const SystemConfig &config, const DeviceType &type)
: ProcessorDevice(config, type)
{

}
