/*
 * mx.cpp - Epson MX80/MX100 printer emulation package
 *
 *  Created on: Mar 26, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/addrmap.h"
#include "emu/map/map.h"
#include "emu/devsys.h"
#include "emu/driver.h"
#include "emu/machine.h"
#include "emu/engine.h"

#include "printers/epson/mx.h"

using namespace aspace;

CommandStatus mx_SetMemory(Console *user, Machine *sys, Device *dev, args_t &args)
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

devCommand_t mx_Commands[] =
{
		{ "mem", mx_SetMemory },

		// null terminal
		nullptr
};

// Create system routines
void mx_prDevice::mx80(SystemConfig &config)
{

	setCommands(mx_Commands);

	cpu = i8049(config, "cpu", 0);
	cpu->setAddressMap(AS_PROGRAM, &mx_prDevice::mx_sbus);


//	cssc   = CSSC(config, "cssc", 0);
////	cpu->setSystemSupport(cssc);
//	cmctl = CMCTL(config, "cmctl", 0);
//	cqbic = CQBIC(config, "cqbic", 0);
//
//	cssc->setDeviceName("cssc");
//	cmctl->setDeviceName("cmctl");
//	cqbic->setDeviceName("cqbic");

}

void mx_prDevice::mx100(SystemConfig &config)
{

	cpu = i8049(config, "cpu", 0);
	cpu->setAddressMap(AS_PROGRAM, &mx_prDevice::mx_sbus);

//	cssc   = CSSC(config, "cssc", 0);
////	cpu->setSystemSupport(cssc);
//	cmctl = CMCTL(config, "cmctl", 0);
//	cqbic = CQBIC(config, "cqbic", 0);
//
//	cssc->setDeviceName("cssc");
//	cmctl->setDeviceName("cmctl");
//	cqbic->setDeviceName("cqbic");

}

// Initialize system routines
void mx_prDevice::mx80_init()
{

}

void mx_prDevice::mx100_init()
{

}


void mx_prDevice::mx_sbus(AddressList &map)
{
	map(0x000, 0x7FF).ram().region("main"); // 2048 bytes
}

static const romEntry_t ROM_NAME(mx80)[] =
{
	ROM_REGION("mx80fw", 0x800, 0),
//	ROM_LOAD("graftrax1.rom", 0x000, 0x7FF, 0, nullptr),
//	ROM_LOAD("graftrax2.rom", 0x000, 0x7FF, 0, nullptr),
	ROM_LOAD("graftrax3.rom", 0x000, 0x7FF, 0, nullptr),
	ROM_END
};

static const romEntry_t ROM_NAME(mx100)[] =
{
	ROM_END
};

PRINTER(mx80,   nullptr, epson, MX80,  mx_prDevice, mx80,  mx80_init,  "Epson", "MX80", SYSTEM_NOT_WORKING)
PRINTER(mx100,  nullptr, epson, MX100, mx_prDevice, mx100, mx100_init, "Epson", "MX100", SYSTEM_NOT_WORKING)


