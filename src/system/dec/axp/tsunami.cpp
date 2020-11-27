/*
 * tsunami.cpp - AlphaServer system (tsunami family)
 *
 *  Created on: Nov 22, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/devsys.h"
#include "emu/driver.h"

#include "system/dec/axp/tsunami.h"

// Create system routines
void tsunami_device::es40(SystemConfig &config)
{

//	cpu = AXP21264CB(config, "21264CB", 0);
//	cpu->setAddressMap(AS_PROGRAM, &ka650_device::mv3900_mem);
//	cpu->setDeviceName("cpu");
//
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

COMP(es40,  nullptr, axp, ES40, tsunami_device, es40,  es40_init,  "DEC", "AlphaServer ES40")
