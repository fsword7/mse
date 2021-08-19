/*
 * vt100.cpp - DEC VT100 terminal
 *
 *  Created on: Aug 19, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/addrmap.h"
#include "emu/map/map.h"
#include "emu/devsys.h"
#include "emu/driver.h"
#include "emu/machine.h"
#include "emu/engine.h"

#include "dev/cpu/i8080/i8080.h"
#include "terminals/dec/vt100.h"

using namespace aspace;

void vt100_vtDevice::vt100(SystemConfig &config)
{

    cpu = i8080(config, "cpu", 0);
    cpu->setAddressMap(AS_PROGRAM, &vt100_vtDevice::vt100_mem);
    cpu->setAddressMap(AS_IOPORT, &vt100_vtDevice::vt100_iomem);
    
}

void vt100_vtDevice::vt100_init()
{

}

void vt100_vtDevice::vt100_mem(AddressList &map)
{

}

void vt100_vtDevice::vt100_iomem(AddressList &map)
{

}

static const romEntry_t ROM_NAME(vt100)[] =
{
	ROM_END
};

TERMINAL(vt100,   nullptr, dec, VT100,  vt100_vtDevice, vt100,  vt100_init,  "DEC", "VT100 terminal", SYSTEM_NOT_WORKING)
