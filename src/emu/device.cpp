/*
 * device.cpp - device handling package
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"

Device::Device(const SystemConfig &config, const DeviceType &type)
: type(type)
{
	ifList.clear();
}

DeviceInterface::DeviceInterface(device_t *owner, const tag_t *name)
: owner(owner), tagName(name)
{

}
