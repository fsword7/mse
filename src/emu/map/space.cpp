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

template <int aWidth, int aShift, int endian>
void AddressSpaceAccess<aWidth, aShift, endian>::createMainMemory(offs_t size)
{
	memData = new uint8_t[size];
	memSize = size;
	memset(memData, 0xE5, size);
}

// **** Read access function calls
template <int aWidth, int aShift, int endian>
uint8_t AddressSpaceAccess<aWidth, aShift, endian>::read8(offs_t addr, ProcessorDevice *cpu)
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

template <int aWidth, int aShift, int endian>
uint16_t AddressSpaceAccess<aWidth, aShift, endian>::read16(offs_t addr, ProcessorDevice *cpu)
{
	if (addr < memSize) {
		uint8_t *ptr = memData + (addr & ~0x1);
		return *((uint16_t *)ptr);
	}
	return 0;
}

template <int aWidth, int aShift, int endian>
uint16_t AddressSpaceAccess<aWidth, aShift, endian>::read16u(offs_t addr, ProcessorDevice *cpu)
{
	return 0;
}

template <int aWidth, int aShift, int endian>
uint32_t AddressSpaceAccess<aWidth, aShift, endian>::read32(offs_t addr, ProcessorDevice *cpu)
{
	if (addr < memSize) {
		uint8_t *ptr = memData + (addr & ~0x3);
		return *((uint32_t *)ptr);
	}
	return 0;
}

template <int aWidth, int aShift, int endian>
uint32_t AddressSpaceAccess<aWidth, aShift, endian>::read32u(offs_t addr, ProcessorDevice *cpu)
{
	return 0;
}

template <int aWidth, int aShift, int endian>
uint64_t AddressSpaceAccess<aWidth, aShift, endian>::read64(offs_t addr, ProcessorDevice *cpu)
{
	if (addr < memSize) {
		uint8_t *ptr = memData + (addr & ~0x7);
		return *((uint64_t *)ptr);
	}
	return 0;
}

template <int aWidth, int aShift, int endian>
uint64_t AddressSpaceAccess<aWidth, aShift, endian>::read64u(offs_t addr, ProcessorDevice *cpu)
{
	return 0;
}

// **** Write access function calls ****

template <int aWidth, int aShift, int endian>
void AddressSpaceAccess<aWidth, aShift, endian>::write8(offs_t addr, uint8_t data, ProcessorDevice *cpu)
{

}

template <int aWidth, int aShift, int endian>
void AddressSpaceAccess<aWidth, aShift, endian>::write16(offs_t addr, uint16_t data, ProcessorDevice *cpu)
{

}

template <int aWidth, int aShift, int endian>
void AddressSpaceAccess<aWidth, aShift, endian>::write16u(offs_t addr, uint16_t data, ProcessorDevice *cpu)
{

}

template <int aWidth, int aShift, int endian>
void AddressSpaceAccess<aWidth, aShift, endian>::write32(offs_t addr, uint32_t data, ProcessorDevice *cpu)
{

}

template <int aWidth, int aShift, int endian>
void AddressSpaceAccess<aWidth, aShift, endian>::write32u(offs_t addr, uint32_t data, ProcessorDevice *cpu)
{

}

template <int aWidth, int aShift, int endian>
void AddressSpaceAccess<aWidth, aShift, endian>::write64(offs_t addr, uint64_t data, ProcessorDevice *cpu)
{

}

template <int aWidth, int aShift, int endian>
void AddressSpaceAccess<aWidth, aShift, endian>::write64u(offs_t addr, uint64_t data, ProcessorDevice *cpu)
{

}

// *********************************************************************

struct mapSpaceInfo
{
	cstag_t name;
	mapSpaceType type;
} asSpaceType[] =
{
		{ "program", mapSpaceType::asProgram },
		{ "data",    mapSpaceType::asData },
		{ "I/O port", mapSpaceType::asPort }
};

void BusManager::allocate(Console *cty, diExternalBus &bus)
{
	Device *dev = bus.getDevice();
	int width;

	fmt::printf("%s: Allocating address space (%d space%s)\n",
		dev->getDeviceName(), bus.getAddressConfigCount(),
		bus.getAddressConfigCount() != 1 ? "s" : "");

	for (int space = 0; space < bus.getAddressConfigCount(); space++)
	{
		const mapAddressConfig *config = bus.getAddressConfig(asSpaceType[space].type);
		if (config == nullptr)
			continue;
		fmt::printf("%s: Allocating %s address space...\n",
			dev->getDeviceName(), asSpaceType[space].name);

		switch (config->getEndianType())
		{
		case LittleEndian:
			width = config->getAddrWidth();
			switch (width) {

			case 8:  bus.allocate<map::AddressSpaceAccess<8,  0, LittleEndian>>(*this, space);  break;
			case 16: bus.allocate<map::AddressSpaceAccess<16, 0, LittleEndian>>(*this, space);  break;
			case 32: bus.allocate<map::AddressSpaceAccess<32, 0, LittleEndian>>(*this, space);  break;
			case 64: bus.allocate<map::AddressSpaceAccess<64, 0, LittleEndian>>(*this, space);  break;

			default:
				fmt::printf("%s: Invalid address configuration - address %d width %d shift\n",
					dev->getDeviceName(), config->getAddrWidth(), config->getAddrShift());
				break;
			}
			break;

		case BigEndian:
		default:
			fmt::printf("%s: Invalid endian type\n", dev->getDeviceName());
			break;
		}
	}
}
