/*
 * dibus.cpp - bus interface package
 *
 *  Created on: Dec 16, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/dibus.h"

diExternalBus::diExternalBus(device_t *owner, ctag_t *name)
: DeviceInterface(owner, name)
{
	owner->ifBus = this;

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

const mapAddressConfig *diExternalBus::getAddressConfig(mapSpaceType type)
{
	if (type >= 0 && type < mapConfig.size())
		return mapConfig[type];
	return nullptr;
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
