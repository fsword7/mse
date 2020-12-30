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

uint8_t diBus::read8(offs_t addr, ProcessorDevice *cpu)
{

	if (addr < memSize)
		return memData[addr];

	// Check memory block ranges
//	for (int idx = 0; idx < mapMemories.size(); idx++)
//	{
//		mapMemory *mem = mapMemories[idx];
//
//		if (addr >= mem->baseAddr && addr < mem->endAddr)
//			return mem->data[addr - mem->baseAddr];
//	}

	return 0;
}

uint16_t diBus::read16(offs_t addr, ProcessorDevice *cpu)
{
	if (addr < memSize) {
		uint8_t *ptr = memData + (addr & ~0x1);
		return *((uint16_t *)ptr);
	}
	return 0;
}

uint32_t diBus::read32(offs_t addr, ProcessorDevice *cpu)
{
	if (addr < memSize) {
		uint8_t *ptr = memData + (addr & ~0x3);
		return *((uint32_t *)ptr);
	}
	return 0;
}

uint64_t diBus::read64(offs_t addr, ProcessorDevice *cpu)
{
	if (addr < memSize) {
		uint8_t *ptr = memData + (addr & ~0x7);
		return *((uint64_t *)ptr);
	}
	return 0;
}
