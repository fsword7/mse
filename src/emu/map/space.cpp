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

namespace map {

	template <int dWidth, int aShift, int endian>
	class AddressSpaceAccess : public AddressSpace
	{
	public:
		AddressSpaceAccess(BusManager &manager, diExternalBus &bus, int space, int addrWidth)
		: AddressSpace(manager, bus, space)
		{
		}

		void createMainMemory(offs_t size)
		{
			memData = new uint8_t[size];
			memSize = size;
			memset(memData, 0xE5, size);
		}

		// **** Read access function calls

		uint8_t read8(offs_t addr, ProcessorDevice *cpu)
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

		uint16_t read16(offs_t addr, ProcessorDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x1);
				return *((uint16_t *)ptr);
			}
			return 0;
		}

		uint16_t read16u(offs_t addr, ProcessorDevice *cpu)
		{
			return 0;
		}

		uint32_t read32(offs_t addr, ProcessorDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x3);
				return *((uint32_t *)ptr);
			}
			return 0;
		}

		uint32_t read32u(offs_t addr, ProcessorDevice *cpu)
		{
			return 0;
		}

		uint64_t read64(offs_t addr, ProcessorDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x7);
				return *((uint64_t *)ptr);
			}
			return 0;
		}

		uint64_t read64u(offs_t addr, ProcessorDevice *cpu)
		{
			return 0;
		}

		// **** Write access function calls ****

		void write8(offs_t addr, uint8_t data, ProcessorDevice *cpu)
		{

		}

		void write16(offs_t addr, uint16_t data, ProcessorDevice *cpu)
		{

		}

		void write16u(offs_t addr, uint16_t data, ProcessorDevice *cpu)
		{

		}

		void write32(offs_t addr, uint32_t data, ProcessorDevice *cpu)
		{

		}

		void write32u(offs_t addr, uint32_t data, ProcessorDevice *cpu)
		{

		}

		void write64(offs_t addr, uint64_t data, ProcessorDevice *cpu)
		{

		}

		void write64u(offs_t addr, uint64_t data, ProcessorDevice *cpu)
		{

		}

	private:
		uint8_t *memData = nullptr;
		offs_t   memSize = 0;
	};

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

//	fmt::printf("%s: Allocating address space (%d space%s)\n",
//		dev->getDeviceName(), bus.getAddressConfigCount(),
//		bus.getAddressConfigCount() != 1 ? "s" : "");

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
			switch (config->getDataWidth() | (config->getAddrShift() & 0x3)) {

			// 8-bit data access
			case 8:  bus.allocate<map::AddressSpaceAccess<8,  0, LittleEndian>>(*this, space);  break;

			// 16-bit data access
			case 16: bus.allocate<map::AddressSpaceAccess<16, 0, LittleEndian>>(*this, space);  break;
			case 17: bus.allocate<map::AddressSpaceAccess<16, 1, LittleEndian>>(*this, space);  break;

			// 32-bit data access
			case 32: bus.allocate<map::AddressSpaceAccess<32, 0, LittleEndian>>(*this, space);  break;
			case 33: bus.allocate<map::AddressSpaceAccess<32, 1, LittleEndian>>(*this, space);  break;
			case 34: bus.allocate<map::AddressSpaceAccess<32, 2, LittleEndian>>(*this, space);  break;

			// 64-bit data access
			case 64: bus.allocate<map::AddressSpaceAccess<64, 0, LittleEndian>>(*this, space);  break;
			case 65: bus.allocate<map::AddressSpaceAccess<64, 1, LittleEndian>>(*this, space);  break;
			case 66: bus.allocate<map::AddressSpaceAccess<64, 2, LittleEndian>>(*this, space);  break;
			case 67: bus.allocate<map::AddressSpaceAccess<64, 3, LittleEndian>>(*this, space);  break;

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
