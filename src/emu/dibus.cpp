/*
 * dibus.cpp - bus interface package
 *
 *  Created on: Dec 16, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/dibus.h"

diBus::diBus(device_t *owner)
: DeviceInterface(owner, "bus")
{
}
