/*
 * tsunami.cpp - AlphaServer system (tsunami family)
 *
 *  Created on: Nov 22, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/addrmap.h"
#include "emu/map/map.h"
#include "emu/devsys.h"
#include "emu/driver.h"

#include "devices/cpu/alpha/axp.h"
#include "devices/cpu/alpha/21264cb.h"
#include "system/dec/axp/tsunami.h"

using namespace aspace;

// Create system routines
void tsunami_device::es40(SystemConfig &config)
{

	for (int idx = 0; idx < ES40_NCPU; idx++)
	{
		string tagName = fmt::sprintf("cpu%d", idx);
		cpu[idx] = axp21264cb(config, tagName, 0);
		cpu[idx]->setAddressMap(AS_PROGRAM, &tsunami_device::es40_sbus);
	}

	// TODO: Removed later when implement
	// 'set <system> memory <size> <region>' command
	// Initialize main memory (memory region)
//	busManager->allocateRegion("main", 19u << 1, 8, LittleEndian);

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

void tsunami_device::es45(SystemConfig &config)
{

	for (int idx = 0; idx < ES40_NCPU; idx++)
	{
		string tagName = fmt::sprintf("cpu%d", idx);
		cpu[idx] = axp21264cb(config, tagName, 0);
		cpu[idx]->setAddressMap(AS_PROGRAM, &tsunami_device::es40_sbus);
	}

	// TODO: Removed later when implement
	// 'set <system> memory <size> <region>' command
	// Initialize main memory (memory region)
//	busManager->allocateRegion("main", 19u << 1, 8, LittleEndian);

//	cssc   = CSSC(config, "cssc", 0);
////	cpu->setSystemSupport(cssc);
//	cmctl = CMCTL(config, "cmctl", 0);
//	cqbic = CQBIC(config, "cqbic", 0);
//
//	cssc->setDeviceName("cssc");
//	cmctl->setDeviceName("cmctl");
//	cqbic->setDeviceName("cqbic");

	cout << "This is AlphaServer ES45 system" << endl;

}

// Initialize system routines
void tsunami_device::es40_init()
{

}

void tsunami_device::es45_init()
{

}

void tsunami_device::es40_sbus(AddressList &map)
{
	map(0x00000000000LL, 0x0001FFFFFFFLL).ram().region("main"); // up to 4 GB main memory (default 512 MB space)
}

void tsunami_device::es45_sbus(AddressList &map)
{
	map(0x00000000000LL, 0x0001FFFFFFFLL).ram().region("main"); // up to 32 GB main memory (default 512 MB space)
}


COMP(es40,  nullptr, axp, ES40, tsunami_device, es40,  es40_init,  "DEC", "AlphaServer ES40")
COMP(es45,  nullptr, axp, ES45, tsunami_device, es45,  es45_init,  "DEC", "AlphaServer ES45")
