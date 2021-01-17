/*
 * dibus.cpp - bus interface package
 *
 *  Created on: Dec 16, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/dibus.h"

using namespace aspace;

diExternalBus::diExternalBus(device_t *owner, ctag_t *name)
: DeviceInterface(owner, name)
{
	owner->ifBus = this;

	AddressMapList.clear();
	mapConfig.clear();
	mapSpace.clear();
//	mapMemories.clear();
}

diExternalBus::~diExternalBus()
{
	// Release all memory blocks
//	for (int idx = 0; idx < mapMemories.size(); idx++)
//		delete mapMemories[idx];
//	mapMemories.clear();

	// Release all addressing configurations and spaces
	mapConfig.clear();
	mapSpace.clear();
}

void diExternalBus::completeConfig()
{
	mapConfigList list = getAddressConfigList();

	for (const auto &entry : list)
	{
		if (entry.type >= mapConfig.size())
			mapConfig.resize(entry.type + 1);
		mapConfig[entry.type] = entry.config;
	}
}

const mapAddressConfig *diExternalBus::getAddressConfig(int space) const
{
	if (space >= 0 && space < mapConfig.size())
		return mapConfig[space];
	return nullptr;
}

AddressSpace *diExternalBus::getAddressSpace(int space) const
{
	if (space >= 0 && space < mapSpace.size())
		return mapSpace[space];
	return nullptr;
}

Constructor diExternalBus::getAddressMap(int space) const
{
	if (space < AddressMapList.size())
		return AddressMapList[space];
	return Constructor();
}

void diExternalBus::setAddressMap(int space, Constructor map)
{
	ctag_t *name = map.getName();

	fmt::printf("Binded object = %s\n", name);
	if (space >= AddressMapList.size())
		AddressMapList.resize(space+1);
	AddressMapList[space] = move(map);
}

//void diBus::registerMemory(mapMemoryBlock *block, offs_t base, offs_t size, int prio)
//{
//	if (mapMemories.size() > 0)
//	{
//
//	}
//
//	mapMemory *mem = new mapMemory;
//
//	mem->baseAddr = base;
//	mem->endAddr  = base + size;
//	mem->length   = size;
//	mem->data     = block->getData();
//
//	mapMemories.push_back(mem);
//}
