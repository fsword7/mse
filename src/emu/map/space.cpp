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
	
	template <int Level, int dWidth, int aShift, endian_t eType>
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

			// Assigned unmapped/nop dispatch calls
			unmapRead = new HandlerReadUnmapped<dWidth, aShift>(this);
			unmapWrite = new HandlerWriteUnmapped<dWidth, aShift>(this);
			nopRead = new HandlerReadNop<dWidth, aShift>(this);
			nopWrite = new HandlerWriteNop<dWidth, aShift>(this);

			HandlerEntry::range r = { 0, static_cast<offs_t>(~0ull >> ((sizeof(offs_t) * 8) - addrWidth)) };

			// switch (addrWidth)
			// {
			// 	case 4:
			// 		rootRead = new HandlerReadDispatch<4, dWidth, aShift>(this, r, nullptr);
			// 		rootWrite = new HandlerWriteDispatch<4, dWidth, aShift>(this, r, nullptr);
			// 		break;

			// 	default:
			// 		fmt::printf("%s: Invalid dispatch handler - address %d width %d shift\n",
			// 			dev->getDeviceName(), addrWidth, aShift);
			// 		break;
			// }

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
		int eType = config->getEndianType() == BigEndian ? 0x0400 : 0x0000;

		switch (eType | (level << 8) | config->getDataWidth() | (config->getAddrShift() & 0x3))
		{
			// 0-bit data access
			case 0x0000 | 0x0000 | 8:    bus.allocate<AddressSpaceSpecific<0, 0, 0, LittleEndian>>(*this, space); break;

			case 0x0000 | 0x0100 | 8:    bus.allocate<AddressSpaceSpecific<1, 0, 0, LittleEndian>>(*this, space); break;

			case 0x0400 | 0x0000 | 8:    bus.allocate<AddressSpaceSpecific<0, 0, 0, BigEndian>>(*this, space); break;

			case 0x0400 | 0x0100 | 8:    bus.allocate<AddressSpaceSpecific<1, 0, 0, BigEndian>>(*this, space); break;

			// 16-bit data access
			case 0x0000 | 0x0000 | 16+0: bus.allocate<AddressSpaceSpecific<0, 1, 0, LittleEndian>>(*this, space); break;
			case 0x0000 | 0x0000 | 16+1: bus.allocate<AddressSpaceSpecific<0, 1, 1, LittleEndian>>(*this, space); break;

			case 0x0000 | 0x0100 | 16+0: bus.allocate<AddressSpaceSpecific<1, 1, 0, LittleEndian>>(*this, space); break;
			case 0x0000 | 0x0100 | 16+1: bus.allocate<AddressSpaceSpecific<1, 1, 1, LittleEndian>>(*this, space); break;

			case 0x0400 | 0x0000 | 16+0: bus.allocate<AddressSpaceSpecific<0, 1, 0, BigEndian>>(*this, space); break;
			case 0x0400 | 0x0000 | 16+1: bus.allocate<AddressSpaceSpecific<0, 1, 1, BigEndian>>(*this, space); break;

			case 0x0400 | 0x0100 | 16+0: bus.allocate<AddressSpaceSpecific<1, 1, 0, BigEndian>>(*this, space); break;
			case 0x0400 | 0x0100 | 16+1: bus.allocate<AddressSpaceSpecific<1, 1, 1, BigEndian>>(*this, space); break;

			// 32-bit data access
			case 0x0000 | 0x0000 | 32+0: bus.allocate<AddressSpaceSpecific<0, 2, 0, LittleEndian>>(*this, space); break;
			case 0x0000 | 0x0000 | 32+1: bus.allocate<AddressSpaceSpecific<0, 2, 1, LittleEndian>>(*this, space); break;
			case 0x0000 | 0x0000 | 32+2: bus.allocate<AddressSpaceSpecific<0, 2, 2, LittleEndian>>(*this, space); break;

			case 0x0000 | 0x0100 | 32+0: bus.allocate<AddressSpaceSpecific<1, 2, 0, LittleEndian>>(*this, space); break;
			case 0x0000 | 0x0100 | 32+1: bus.allocate<AddressSpaceSpecific<1, 2, 1, LittleEndian>>(*this, space); break;
			case 0x0000 | 0x0100 | 32+2: bus.allocate<AddressSpaceSpecific<1, 2, 2, LittleEndian>>(*this, space); break;

			case 0x0400 | 0x0000 | 32+0: bus.allocate<AddressSpaceSpecific<0, 2, 0, BigEndian>>(*this, space); break;
			case 0x0400 | 0x0000 | 32+1: bus.allocate<AddressSpaceSpecific<0, 2, 1, BigEndian>>(*this, space); break;
			case 0x0400 | 0x0000 | 32+2: bus.allocate<AddressSpaceSpecific<0, 2, 2, BigEndian>>(*this, space); break;

			case 0x0400 | 0x0100 | 32+0: bus.allocate<AddressSpaceSpecific<1, 2, 0, BigEndian>>(*this, space); break;
			case 0x0400 | 0x0100 | 32+1: bus.allocate<AddressSpaceSpecific<1, 2, 1, BigEndian>>(*this, space); break;
			case 0x0400 | 0x0100 | 32+2: bus.allocate<AddressSpaceSpecific<1, 2, 2, BigEndian>>(*this, space); break;

			// 64-bit data access
			case 0x0000 | 0x0000 | 64+0: bus.allocate<AddressSpaceSpecific<0, 3, 0, LittleEndian>>(*this, space); break;
			case 0x0000 | 0x0000 | 64+1: bus.allocate<AddressSpaceSpecific<0, 3, 1, LittleEndian>>(*this, space); break;
			case 0x0000 | 0x0000 | 64+2: bus.allocate<AddressSpaceSpecific<0, 3, 2, LittleEndian>>(*this, space); break;
			case 0x0000 | 0x0000 | 64+3: bus.allocate<AddressSpaceSpecific<0, 3, 3, LittleEndian>>(*this, space); break;

			case 0x0000 | 0x0100 | 64+0: bus.allocate<AddressSpaceSpecific<1, 3, 0, LittleEndian>>(*this, space); break;
			case 0x0000 | 0x0100 | 64+1: bus.allocate<AddressSpaceSpecific<1, 3, 1, LittleEndian>>(*this, space); break;
			case 0x0000 | 0x0100 | 64+2: bus.allocate<AddressSpaceSpecific<1, 3, 2, LittleEndian>>(*this, space); break;
			case 0x0000 | 0x0100 | 64+3: bus.allocate<AddressSpaceSpecific<1, 3, 3, LittleEndian>>(*this, space); break;

			case 0x0400 | 0x0000 | 64+0: bus.allocate<AddressSpaceSpecific<0, 3, 0, BigEndian>>(*this, space); break;
			case 0x0400 | 0x0000 | 64+1: bus.allocate<AddressSpaceSpecific<0, 3, 1, BigEndian>>(*this, space); break;
			case 0x0400 | 0x0000 | 64+2: bus.allocate<AddressSpaceSpecific<0, 3, 2, BigEndian>>(*this, space); break;
			case 0x0400 | 0x0000 | 64+3: bus.allocate<AddressSpaceSpecific<0, 3, 3, BigEndian>>(*this, space); break;

			case 0x0400 | 0x0100 | 64+0: bus.allocate<AddressSpaceSpecific<1, 3, 0, BigEndian>>(*this, space); break;
			case 0x0400 | 0x0100 | 64+1: bus.allocate<AddressSpaceSpecific<1, 3, 1, BigEndian>>(*this, space); break;
			case 0x0400 | 0x0100 | 64+2: bus.allocate<AddressSpaceSpecific<1, 3, 2, BigEndian>>(*this, space); break;
			case 0x0400 | 0x0100 | 64+3: bus.allocate<AddressSpaceSpecific<1, 3, 3, BigEndian>>(*this, space); break;

			default:
				fmt::printf("%s: Invalid address configuration - address %d width %d shift\n",
					dev->getDeviceName(), config->getAddrWidth(), config->getAddrShift());
				break;
		}


	}
}
