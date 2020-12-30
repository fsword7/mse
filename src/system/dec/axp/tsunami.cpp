/*
 * tsunami.cpp - AlphaServer system (tsunami family)
 *
 *  Created on: Nov 22, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/devsys.h"
#include "emu/driver.h"

#include "devices/cpu/alpha/axp.h"
#include "devices/cpu/alpha/21264cb.h"
#include "system/dec/axp/tsunami.h"

// Create system routines
void tsunami_device::es40(SystemConfig &config)
{
	// Initialize memory (removed later)
	memSize = 19u << 1; // 512 MB memory space
	memData = new uint8_t[memSize];
	memset(memData, 0, memSize);

	for (int idx = 0; idx < ES40_NCPU; idx++)
	{
		string tagName = fmt::sprintf("cpu%d", idx);
		cpu[idx] = axp21264cb(config, tagName, 0);
//		cpu[idx]->setAddressMap(AS_PROGRAM, &tsunami_device::es40_mem);
	}

//	cssc   = CSSC(config, "cssc", 0);
////	cpu->setSystemSupport(cssc);
//	cmctl = CMCTL(config, "cmctl", 0);
//	cqbic = CQBIC(config, "cqbic", 0);
//
//	cssc->setDeviceName("cssc");
//	cmctl->setDeviceName("cmctl");
//	cqbic->setDeviceName("cqbic");

	cout << "This is AlphaServer ES40 system" << endl;

}

// Initialize system routines
void tsunami_device::es40_init()
{

}

uint8_t tsunami_device::read8(offs_t addr)
{
	if (addr < memSize)
		return memData[addr];
	return 0;
}

void tsunami_device::write8(offs_t addr, uint8_t data)
{
	if (addr < memSize)
		memData[addr] = data;
}

COMP(es40,  nullptr, axp, ES40, tsunami_device, es40,  es40_init,  "DEC", "AlphaServer ES40")
