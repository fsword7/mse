/*
 * busmgr.cpp - external bus manager package
 *
 *  Created on: Jan 2, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/machine.h"
#include "emu/dibus.h"
#include "emu/map/addrmap.h"
#include "emu/map/map.h"

using namespace map;

void BusManager::allocate(Console *cty, diExternalBus &bus)
{
	fmt::printf("%s: Allocating space\n", bus.getDevice()->getDeviceName());
}

void BusManager::init(Console *cty)
{
	fmt::printf("%s: External bus initialization\n", system->getDeviceName());
	cout << flush;

	InterfaceIterator<diExternalBus> iter(*system->getSystemDevice());
	vector<diExternalBus *> buses;

	// Find each device that has external bus system
	buses.clear();
	for (diExternalBus &bus : iter) {
		buses.push_back(&bus);
		allocate(cty, bus);
	}

}
