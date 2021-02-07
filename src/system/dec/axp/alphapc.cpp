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

#include "system/dec/axp/alphapc.h"

using namespace aspace;

CommandStatus alphapc_SetMemory(Console *user, Machine *sys, Device *dev, args_t &args)
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

devCommand_t alphapc_Commands[] =
{
		{ "mem", alphapc_SetMemory },

		// null terminal
		nullptr
};

// Create system routines
void alphapc_sysDevice::pc164(SystemConfig &config)
{

	setCommands(alphapc_Commands);

	cpu = dec21164(config, "cpu", 0);
	cpu->setAddressMap(AS_PROGRAM, &alphapc_sysDevice::pc164_sbus);


//	cssc   = CSSC(config, "cssc", 0);
////	cpu->setSystemSupport(cssc);
//	cmctl = CMCTL(config, "cmctl", 0);
//	cqbic = CQBIC(config, "cqbic", 0);
//
//	cssc->setDeviceName("cssc");
//	cmctl->setDeviceName("cmctl");
//	cqbic->setDeviceName("cqbic");

}

void alphapc_sysDevice::pc164lx(SystemConfig &config)
{

	cpu = dec21164(config, "cpu", 0);
	cpu->setAddressMap(AS_PROGRAM, &alphapc_sysDevice::pc164lx_sbus);

//	cssc   = CSSC(config, "cssc", 0);
////	cpu->setSystemSupport(cssc);
//	cmctl = CMCTL(config, "cmctl", 0);
//	cqbic = CQBIC(config, "cqbic", 0);
//
//	cssc->setDeviceName("cssc");
//	cmctl->setDeviceName("cmctl");
//	cqbic->setDeviceName("cqbic");

}

void alphapc_sysDevice::pc164sx(SystemConfig &config)
{

	cpu = dec21164(config, "cpu", 0);
	cpu->setAddressMap(AS_PROGRAM, &alphapc_sysDevice::pc164sx_sbus);

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
void alphapc_sysDevice::pc164_init()
{

}

void alphapc_sysDevice::pc164lx_init()
{

}

void alphapc_sysDevice::pc164sx_init()
{

}

void alphapc_sysDevice::pc164_sbus(AddressList &map)
{
	map(0x0000000000LL, 0x00FFFFFFFFLL).ram().region("main"); // up to 512 MB main memory
}

void alphapc_sysDevice::pc164lx_sbus(AddressList &map)
{
	map(0x0000000000LL, 0x07FFFFFFFFLL).ram().region("main"); // up to 512 MB main memory
}

void alphapc_sysDevice::pc164sx_sbus(AddressList &map)
{
	map(0x0000000000LL, 0x07FFFFFFFFLL).ram().region("main"); // up to 512 MB main memory
}

COMP(pc164,    nullptr, axp, PC164, alphapc_sysDevice, pc164,  pc164_init,  "DEC", "AlphaPC 164")
COMP(pc164lx,  nullptr, axp, PC164LX, alphapc_sysDevice, pc164lx,  pc164lx_init,  "DEC", "AlphaPC 164LX")
COMP(pc164sx,  nullptr, axp, PC164SX, alphapc_sysDevice, pc164sx,  pcl64sx_init,  "DEC", "AlphaPC 164SX")
