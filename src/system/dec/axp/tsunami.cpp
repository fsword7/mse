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
#include "emu/machine.h"
#include "emu/engine.h"

#include "dev/cpu/alpha/axp.h"
#include "dev/cpu/alpha/ev6.h"
#include "system/dec/axp/tsunami.h"

using namespace aspace;

CommandStatus es40_SetMemory(Console *user, Machine *sys, Device *dev, args_t &args)
{
	BusManager &manager = sys->getExternalBusManager();
	SystemEngine engine;

	string name = args.getNext();
	string size = args.getNext();

//	fmt::printf("%s: Region: name=%s size=%s\n", dev->getDeviceName(), name, size);

	uint64_t val = engine.getValue(size);

	manager.allocateRegion(name, val, 8, LittleEndian);
	user->printf("%s: Successfully allocated %lld (%llX) to region '%s'\n",
		dev->getDeviceName(), val, val, name);

	return CommandStatus::cmdOk;
}

devCommand_t es40_Commands[] =
{
		{ "mem", es40_SetMemory },

		// null terminal
		nullptr
};

// Create system routines
void tsunami_sysDevice::es40(SystemConfig &config)
{

	setCommands(es40_Commands);

	for (int idx = 0; idx < ES40_NCPU; idx++)
	{
		string tagName = fmt::sprintf("cpu%d", idx);
		cpu[idx] = dec21264(config, tagName, 0);
		cpu[idx]->setAddressMap(AS_PROGRAM, &tsunami_sysDevice::es40_sbus);
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

void tsunami_sysDevice::es45(SystemConfig &config)
{

	for (int idx = 0; idx < ES40_NCPU; idx++)
	{
		string tagName = fmt::sprintf("cpu%d", idx);
		cpu[idx] = dec21264(config, tagName, 0);
		cpu[idx]->setAddressMap(AS_PROGRAM, &tsunami_sysDevice::es40_sbus);
	}

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
void tsunami_sysDevice::es40_init()
{

}

void tsunami_sysDevice::es45_init()
{

}

void tsunami_sysDevice::es40_sbus(AddressList &map)
{
	map(0x00000000000LL, 0x000FFFFFFFFLL).ram().region("main"); // up to 4 GB main memory (default 512 MB space)
}

void tsunami_sysDevice::es45_sbus(AddressList &map)
{
	map(0x00000000000LL, 0x007FFFFFFFFLL).ram().region("main"); // up to 32 GB main memory (default 512 MB space)
}


COMP(es40,  nullptr, axp, ES40, tsunami_sysDevice, es40,  es40_init,  "DEC", "AlphaServer ES40")
COMP(es45,  nullptr, axp, ES45, tsunami_sysDevice, es45,  es45_init,  "DEC", "AlphaServer ES45")
