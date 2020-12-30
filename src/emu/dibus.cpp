/*
 * dibus.cpp - bus interface package
 *
 *  Created on: Dec 16, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/dibus.h"

diBus::diBus(device_t *owner)
: DeviceInterface(owner, "bus")
{
	owner->ifBus = this;

	mapConfig.clear();
	mapSpace.clear();
	mapMemories.clear();
}

diBus::~diBus()
{
	// Release all memory blocks
	for (int idx = 0; idx < mapMemories.size(); idx++)
		delete mapMemories[idx];
	mapMemories.clear();
}

void diBus::registerMemory(mapMemoryBlock *block, offs_t base, offs_t size, int prio)
{
	if (mapMemories.size() > 0)
	{

	}

	mapMemory *mem = new mapMemory;

	mem->baseAddr = base;
	mem->endAddr  = base + size;
	mem->length   = size;
	mem->data     = block->getData();

	mapMemories.push_back(mem);
}
