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
#include "dev/video/vt100.h"
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
    map.setGlobalAddressMask(0xFFFF);
    map(0x0000, 0x1FFF).rom().region("vt100fw"); // ROM space (4 x 2K ROM chips)
    map(0x2000, 0x3FFF).ram(); // screen and scratch space
    // 0x4000 - 0x7FFF - unassigned space
    map(0x8000, 0x9FFF).rom(); // program expansion (4 x 2K ROM chips)
    map(0xA000, 0xBFFF).rom(); // program expansion (1 x 8K ROM chips)
    // 0xC000 - 0xFFFF - unassigned space
}

void vt100_vtDevice::vt100_iomem(AddressList &map)
{
}

static const romEntry_t ROM_NAME(vt100)[] =
{      
    ROM_REGION("vt100fw", 0x2000, 0),
    ROM_LOAD("23-061e2-00.e56", 0x0000, 0x0800, 0, nullptr),
    ROM_LOAD("23-032e2-00.e52", 0x0800, 0x0800, 0, nullptr),
    ROM_LOAD("23-033e2-00.e45", 0x1000, 0x0800, 0, nullptr),
    ROM_LOAD("23-034e2-00.e40", 0x1800, 0x0800, 0, nullptr),

    ROM_REGION("chargen", 0x0800, 0),
    ROM_LOAD("23-018e2-00.e4", 0x0000, 0x0800, 0, nullptr),

	ROM_END
};

TERMINAL(vt100,   nullptr, dec, VT100,  vt100_vtDevice, vt100,  vt100_init,  "DEC", "VT100 terminal", SYSTEM_NOT_WORKING)
