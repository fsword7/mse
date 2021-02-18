/*
 * nvmem.cpp - Non-volatile memory interface package
 *
 *  Created on: Feb 17, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/chip/nvmem.h"

nvmInterface::nvmInterface(Device *owner, ctag_t *name)
: DeviceInterface(owner, (name != nullptr) ? name : "nvmem")
{

}

nvmInterface::~nvmInterface()
{
	// Save contents
}
