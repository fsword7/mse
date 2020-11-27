/*
 * diexec.cpp - Device Interface - Memory
 *
 *  Created on: Nov 27, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/dimem.h"

diMemory::diMemory(device_t *owner)
: DeviceInterface(owner, "memory")
{

}
