/*
 * space.cpp - addressing space for bus interface
 *
 *  Created on: Dec 30, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/addrmap.h"
#include "emu/map/map.h"
#include "emu/devproc.h"

using namespace map;

void mapAddressSpace::createMainMemory(offs_t size)
{
	memData = new uint8_t[size];
	memSize = size;
	memset(memData, 0xE5, size);
}

// **** Read access function calls

uint8_t mapAddressSpace::read8(offs_t addr, ProcessorDevice *cpu)
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

uint16_t mapAddressSpace::read16(offs_t addr, ProcessorDevice *cpu)
{
	if (addr < memSize) {
		uint8_t *ptr = memData + (addr & ~0x1);
		return *((uint16_t *)ptr);
	}
	return 0;
}

uint16_t mapAddressSpace::read16u(offs_t addr, ProcessorDevice *cpu)
{
	return 0;
}

uint32_t mapAddressSpace::read32(offs_t addr, ProcessorDevice *cpu)
{
	if (addr < memSize) {
		uint8_t *ptr = memData + (addr & ~0x3);
		return *((uint32_t *)ptr);
	}
	return 0;
}

uint32_t mapAddressSpace::read32u(offs_t addr, ProcessorDevice *cpu)
{
	return 0;
}

uint64_t mapAddressSpace::read64(offs_t addr, ProcessorDevice *cpu)
{
	if (addr < memSize) {
		uint8_t *ptr = memData + (addr & ~0x7);
		return *((uint64_t *)ptr);
	}
	return 0;
}

uint64_t mapAddressSpace::read64u(offs_t addr, ProcessorDevice *cpu)
{
	return 0;
}

// **** Write access function calls ****

void mapAddressSpace::write8(offs_t addr, uint8_t data, ProcessorDevice *cpu)
{

}

void mapAddressSpace::write16(offs_t addr, uint16_t data, ProcessorDevice *cpu)
{

}

void mapAddressSpace::write16u(offs_t addr, uint16_t data, ProcessorDevice *cpu)
{

}

void mapAddressSpace::write32(offs_t addr, uint32_t data, ProcessorDevice *cpu)
{

}

void mapAddressSpace::write32u(offs_t addr, uint32_t data, ProcessorDevice *cpu)
{

}

void mapAddressSpace::write64(offs_t addr, uint64_t data, ProcessorDevice *cpu)
{

}

void mapAddressSpace::write64u(offs_t addr, uint64_t data, ProcessorDevice *cpu)
{

}
