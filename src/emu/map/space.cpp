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

using namespace aspace;

namespace aspace {

	template <int dWidth, int aShift, int endian>
	class AddressSpaceAccess : public AddressSpace
	{
		using thisType = AddressSpaceAccess<dWidth, aShift, endian>;

	public:
		AddressSpaceAccess(BusManager &manager, diExternalBus &bus, int space, int addrWidth)
		: AddressSpace(manager, bus, space)
		{
			Device *dev = bus.getDevice();

			HandlerEntry::range r = { 0, static_cast<offs_t>(~0ull >> ((sizeof(offs_t) * 8) - addrWidth)) };

			fmt::printf("%s: Address range: %llX - %llX (%d-bit addressing)\n",
				dev->getDeviceName(), r.start, r.end, addrWidth);
		}

		// **** Read access function calls

		uint8_t read8(offs_t addr, cpuDevice *cpu)
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

			return unmapValue;
		}

		uint16_t read16(offs_t addr, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x1);
				return *((uint16_t *)ptr);
			}
			return unmapValue;
		}

		uint16_t read16u(offs_t addr, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + addr;
				return *((uint16_t *)ptr);
			}
			return unmapValue;
		}

		uint32_t read32(offs_t addr, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x3);
				return *((uint32_t *)ptr);
			}
			return unmapValue;
		}

		uint32_t read32u(offs_t addr, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + addr;
				return *((uint32_t *)ptr);
			}
			return unmapValue;
		}

		uint64_t read64(offs_t addr, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x7);
				return *((uint64_t *)ptr);
			}
			return unmapValue;
		}

		uint64_t read64u(offs_t addr, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + addr;
				return *((uint64_t *)ptr);
			}
			return unmapValue;
		}

		void readBlock(offs_t addr, uint8_t *data, uint64_t size)
		{
			if (addr < memSize) {
				if ((addr + size) > memSize)
					size = memSize - addr;
				memcpy(data, &memData[addr], size);
			}
		}

		// **** Write access function calls ****

		void write8(offs_t addr, uint8_t data, cpuDevice *cpu)
		{
			if (addr < memSize)
				memData[addr] = data;
		}

		void write16(offs_t addr, uint16_t data, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x1);
				*((uint16_t *)ptr) = data;
			}
		}

		void write16u(offs_t addr, uint16_t data, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + addr;
				*((uint16_t *)ptr) = data;
			}
		}

		void write32(offs_t addr, uint32_t data, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x2);
				*((uint32_t *)ptr) = data;
			}
		}

		void write32u(offs_t addr, uint32_t data, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + addr;
				*((uint32_t *)ptr) = data;
			}
		}

		void write64(offs_t addr, uint64_t data, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x3);
				*((uint64_t *)ptr) = data;
			}
		}

		void write64u(offs_t addr, uint64_t data, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + addr;
				*((uint64_t *)ptr) = data;
			}
		}

		void writeBlock(offs_t addr, uint8_t *data, uint64_t size)
		{
			if (addr < memSize) {
				if ((addr + size) > memSize)
					size = memSize - addr;
				memcpy(&memData[addr], data, size);
			}
		}

	private:
		// HandlerRead<dWidth, aShift, endian> *rootRead = nullptr;
		// HandlerWrite<dWidth, aShift, endian> *rootWrite = nullptr;
	};
	
	template <int Level, int dWidth, int aShift, int endian>
	class AddressSpaceSpecific : public AddressSpace
	{
		// using uintx_t = typename HeaderSize<dWidth>::uintx_t;
		// using thisType = AddressSpaceAccess<dWidth, aShift, endian>;
		// using nativeType = uintx_t;

		static constexpr uint64_t nativeBytes = 1 << dWidth;
		static constexpr uint64_t nativeStep = (aShift < 0) ? nativeBytes << labs(aShift) : nativeBytes >> labs(aShift);
		static constexpr uint64_t nativeMask = nativeStep - 1;
		static constexpr uint64_t nativeBits = nativeBytes * 8;

	public:
		AddressSpaceSpecific(BusManager &manager, diExternalBus &bus, int space, int addrWidth)
		: AddressSpace(manager, bus, space)
		{
			Device *dev = bus.getDevice();

			HandlerEntry::range r = { 0, static_cast<offs_t>(~0ull >> ((sizeof(offs_t) * 8) - addrWidth)) };

			fmt::printf("%s: Address range: %llX - %llX (%d-bit addressing)\n",
				dev->getDeviceName(), r.start, r.end, addrWidth);
		}

		// **** Read access function calls

		uint8_t read8(offs_t addr, cpuDevice *cpu)
		{
			if (addr < memSize)
				return memData[addr];

			return unmapValue;
		}

		uint16_t read16(offs_t addr, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x1);
				return *((uint16_t *)ptr);
			}
			return unmapValue;
		}

		uint16_t read16u(offs_t addr, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + addr;
				return *((uint16_t *)ptr);
			}
			return unmapValue;
		}

		uint32_t read32(offs_t addr, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x3);
				return *((uint32_t *)ptr);
			}
			return unmapValue;
		}

		uint32_t read32u(offs_t addr, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + addr;
				return *((uint32_t *)ptr);
			}
			return unmapValue;
		}

		uint64_t read64(offs_t addr, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x7);
				return *((uint64_t *)ptr);
			}
			return unmapValue;
		}

		uint64_t read64u(offs_t addr, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + addr;
				return *((uint64_t *)ptr);
			}
			return unmapValue;
		}

		void readBlock(offs_t addr, uint8_t *data, uint64_t size)
		{
			if (addr < memSize) {
				if ((addr + size) > memSize)
					size = memSize - addr;
				memcpy(data, &memData[addr], size);
			}
		}

		// **** Write access function calls ****

		void write8(offs_t addr, uint8_t data, cpuDevice *cpu)
		{
			if (addr < memSize)
				memData[addr] = data;
		}

		void write16(offs_t addr, uint16_t data, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x1);
				*((uint16_t *)ptr) = data;
			}
		}

		void write16u(offs_t addr, uint16_t data, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + addr;
				*((uint16_t *)ptr) = data;
			}
		}

		void write32(offs_t addr, uint32_t data, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x2);
				*((uint32_t *)ptr) = data;
			}
		}

		void write32u(offs_t addr, uint32_t data, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + addr;
				*((uint32_t *)ptr) = data;
			}
		}

		void write64(offs_t addr, uint64_t data, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + (addr & ~0x3);
				*((uint64_t *)ptr) = data;
			}
		}

		void write64u(offs_t addr, uint64_t data, cpuDevice *cpu)
		{
			if (addr < memSize) {
				uint8_t *ptr = memData + addr;
				*((uint64_t *)ptr) = data;
			}
		}

		void writeBlock(offs_t addr, uint8_t *data, uint64_t size)
		{
			if (addr < memSize) {
				if ((addr + size) > memSize)
					size = memSize - addr;
				memcpy(&memData[addr], data, size);
			}
		}

	private:
		HandlerRead<dWidth, aShift> *rootRead = nullptr;
		HandlerWrite<dWidth, aShift> *rootWrite = nullptr;
	};

	// template <int dWidth, int aShift, int endian>
	// class AddressSpaceCache : public AddressSpace
	// {

	// };

}

// *********************************************************************

using namespace aspace;

ctag_t *asDescrip[] = { "program", "data", "I/O port" };

void BusManager::allocate(Console *cty, diExternalBus &bus)
{
	Device *dev = bus.getDevice();
	int width;

//	fmt::printf("%s: Allocating address space (%d space%s)\n",
//		dev->getDeviceName(), bus.getAddressConfigCount(),
//		bus.getAddressConfigCount() != 1 ? "s" : "");

	for (int space = 0; space < bus.getAddressConfigCount(); space++)
	{
		const mapAddressConfig *config = bus.getAddressConfig(space);
		if (config == nullptr)
			continue;
		fmt::printf("%s: Allocating %s address space...\n",
			dev->getDeviceName(), asDescrip[space]);

		int level = determineDispatchLevel(config->getAddrWidth());

		switch (config->getEndianType())
		{
		case LittleEndian:
			switch (config->getDataWidth() | (config->getAddrShift() & 0x3)) {

			// 8-bit data access
			case 8:  bus.allocate<AddressSpaceAccess<8,  0, LittleEndian>>(*this, space);  break;

			// 16-bit data access
			case 16: bus.allocate<AddressSpaceAccess<16, 0, LittleEndian>>(*this, space);  break;
			case 17: bus.allocate<AddressSpaceAccess<16, 1, LittleEndian>>(*this, space);  break;

			// 32-bit data access
			case 32: bus.allocate<AddressSpaceAccess<32, 0, LittleEndian>>(*this, space);  break;
			case 33: bus.allocate<AddressSpaceAccess<32, 1, LittleEndian>>(*this, space);  break;
			case 34: bus.allocate<AddressSpaceAccess<32, 2, LittleEndian>>(*this, space);  break;

			// 64-bit data access
			case 64: bus.allocate<AddressSpaceAccess<64, 0, LittleEndian>>(*this, space);  break;
			case 65: bus.allocate<AddressSpaceAccess<64, 1, LittleEndian>>(*this, space);  break;
			case 66: bus.allocate<AddressSpaceAccess<64, 2, LittleEndian>>(*this, space);  break;
			case 67: bus.allocate<AddressSpaceAccess<64, 3, LittleEndian>>(*this, space);  break;

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
