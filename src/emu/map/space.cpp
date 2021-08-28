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
		using uintx_t = typename HandlerSize<dWidth>::uintx_t;
		using thisType = AddressSpaceSpecific<Level, dWidth, aShift, eType>;
		using nativeType = uintx_t;

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

			switch (addrWidth)
			{
				case 4:
					rootRead = new HandlerReadDispatch<4, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<4, dWidth, aShift>(this, r, nullptr);
					break;

				case 5:
					rootRead = new HandlerReadDispatch<5, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<5, dWidth, aShift>(this, r, nullptr);
					break;

				case 6:
					rootRead = new HandlerReadDispatch<6, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<6, dWidth, aShift>(this, r, nullptr);
					break;

				case 7:
					rootRead = new HandlerReadDispatch<7, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<7, dWidth, aShift>(this, r, nullptr);
					break;

				case 8:
					rootRead = new HandlerReadDispatch<8, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<8, dWidth, aShift>(this, r, nullptr);
					break;

				case 9:
					rootRead = new HandlerReadDispatch<9, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<9, dWidth, aShift>(this, r, nullptr);
					break;

				case 10:
					rootRead = new HandlerReadDispatch<10, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<10, dWidth, aShift>(this, r, nullptr);
					break;

				case 11:
					rootRead = new HandlerReadDispatch<11, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<11, dWidth, aShift>(this, r, nullptr);
					break;

				case 12:
					rootRead = new HandlerReadDispatch<12, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<12, dWidth, aShift>(this, r, nullptr);
					break;

				case 13:
					rootRead = new HandlerReadDispatch<13, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<13, dWidth, aShift>(this, r, nullptr);
					break;

				case 14:
					rootRead = new HandlerReadDispatch<14, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<14, dWidth, aShift>(this, r, nullptr);
					break;

				case 15:
					rootRead = new HandlerReadDispatch<15, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<15, dWidth, aShift>(this, r, nullptr);
					break;

				case 16:
					rootRead = new HandlerReadDispatch<16, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<16, dWidth, aShift>(this, r, nullptr);
					break;

				case 17:
					rootRead = new HandlerReadDispatch<17, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<17, dWidth, aShift>(this, r, nullptr);
					break;

				case 18:
					rootRead = new HandlerReadDispatch<18, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<18, dWidth, aShift>(this, r, nullptr);
					break;

				case 19:
					rootRead = new HandlerReadDispatch<19, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<19, dWidth, aShift>(this, r, nullptr);
					break;

				case 20:
					rootRead = new HandlerReadDispatch<20, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<20, dWidth, aShift>(this, r, nullptr);
					break;

				case 21:
					rootRead = new HandlerReadDispatch<21, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<21, dWidth, aShift>(this, r, nullptr);
					break;

				case 22:
					rootRead = new HandlerReadDispatch<22, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<22, dWidth, aShift>(this, r, nullptr);
					break;

				case 23:
					rootRead = new HandlerReadDispatch<23, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<23, dWidth, aShift>(this, r, nullptr);
					break;

				case 24:
					rootRead = new HandlerReadDispatch<24, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<24, dWidth, aShift>(this, r, nullptr);
					break;

				case 25:
					rootRead = new HandlerReadDispatch<25, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<25, dWidth, aShift>(this, r, nullptr);
					break;

				case 26:
					rootRead = new HandlerReadDispatch<26, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<26, dWidth, aShift>(this, r, nullptr);
					break;

				case 27:
					rootRead = new HandlerReadDispatch<27, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<27, dWidth, aShift>(this, r, nullptr);
					break;

				case 28:
					rootRead = new HandlerReadDispatch<28, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<28, dWidth, aShift>(this, r, nullptr);
					break;

				case 29:
					rootRead = new HandlerReadDispatch<29, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<29, dWidth, aShift>(this, r, nullptr);
					break;

				case 30:
					rootRead = new HandlerReadDispatch<30, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<30, dWidth, aShift>(this, r, nullptr);
					break;

				case 31:
					rootRead = new HandlerReadDispatch<31, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<31, dWidth, aShift>(this, r, nullptr);
					break;

				case 32:
					rootRead = new HandlerReadDispatch<32, dWidth, aShift>(this, r, nullptr);
					rootWrite = new HandlerWriteDispatch<32, dWidth, aShift>(this, r, nullptr);
					break;

				// case 33:
				// 	rootRead = new HandlerReadDispatch<33, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<33, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 34:
				// 	rootRead = new HandlerReadDispatch<34, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<34, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 35:
				// 	rootRead = new HandlerReadDispatch<35, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<35, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 36:
				// 	rootRead = new HandlerReadDispatch<36, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<36, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 37:
				// 	rootRead = new HandlerReadDispatch<37, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<37, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 38:
				// 	rootRead = new HandlerReadDispatch<38, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<38, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 39:
				// 	rootRead = new HandlerReadDispatch<39, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<39, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 40:
				// 	rootRead = new HandlerReadDispatch<40, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<40, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 41:
				// 	rootRead = new HandlerReadDispatch<41, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<41, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 42:
				// 	rootRead = new HandlerReadDispatch<42, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<42, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 43:
				// 	rootRead = new HandlerReadDispatch<43, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<43, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 44:
				// 	rootRead = new HandlerReadDispatch<44, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<44, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 45:
				// 	rootRead = new HandlerReadDispatch<45, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<45, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 46:
				// 	rootRead = new HandlerReadDispatch<46, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<46, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 47:
				// 	rootRead = new HandlerReadDispatch<47, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<47, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 48:
				// 	rootRead = new HandlerReadDispatch<48, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<48, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 49:
				// 	rootRead = new HandlerReadDispatch<49, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<49, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 50:
				// 	rootRead = new HandlerReadDispatch<50, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<50, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 51:
				// 	rootRead = new HandlerReadDispatch<51, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<51, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 52:
				// 	rootRead = new HandlerReadDispatch<52, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<52, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 53:
				// 	rootRead = new HandlerReadDispatch<53, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<53, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 54:
				// 	rootRead = new HandlerReadDispatch<54, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<54, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 55:
				// 	rootRead = new HandlerReadDispatch<55, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<55, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 56:
				// 	rootRead = new HandlerReadDispatch<56, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<56, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 57:
				// 	rootRead = new HandlerReadDispatch<57, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<57, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 58:
				// 	rootRead = new HandlerReadDispatch<58, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<58, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 59:
				// 	rootRead = new HandlerReadDispatch<59, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<59, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 60:
				// 	rootRead = new HandlerReadDispatch<60, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<60, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 61:
				// 	rootRead = new HandlerReadDispatch<61, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<61, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 62:
				// 	rootRead = new HandlerReadDispatch<62, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<62, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 63:
				// 	rootRead = new HandlerReadDispatch<63, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<63, dWidth, aShift>(this, r, nullptr);
				// 	break;

				// case 64:
				// 	rootRead = new HandlerReadDispatch<64, dWidth, aShift>(this, r, nullptr);
				// 	rootWrite = new HandlerWriteDispatch<64, dWidth, aShift>(this, r, nullptr);
				// 	break;

				default:
					fmt::printf("%s: Invalid dispatch handler - address %d width %d shift\n",
						dev->getDeviceName(), addrWidth, aShift);
					break;
			}

			// Assign dispatch table for direct read/write accesses
			dispatchRead = rootRead->getDispatch();
			dispatchWrite = rootWrite->getDispatch();

			fmt::printf("%s: Address range: %llX - %llX (%d-bit addressing)\n",
				dev->getDeviceName(), r.start, r.end, addrWidth);
		}


		std::pair<const void *, const void *> getSpecificDispatches() override
		{
			std::pair<const void *, const void *> rw;

			rw.first = dispatchRead;
			rw.second = dispatchWrite;

			return rw;
		}

		std::pair<const void *, const void *> getCacheDispatches() override
		{
			std::pair<const void *, const void *> rw;

			rw.first = rootRead;
			rw.second = rootWrite;

			return rw;
		}


		void setMemorySpace(offs_t addrStart, offs_t addrEnd, offs_t addrMirror, uint8_t *data, accessType rwType) override
		{

			if (rwType == accRead)
			{
				auto handler = new HandlerReadMemory<dWidth, aShift>(this, data);

				rootRead->populate(addrStart, addrEnd, addrMirror, handler);
			}

			if (rwType == accWrite)
			{
				auto handler = new HandlerWriteMemory<dWidth, aShift>(this, data);

				rootWrite->populate(addrStart, addrEnd, addrMirror, handler);
			}
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

		// For read/write access function calls
		const HandlerRead<dWidth, aShift> *const *dispatchRead = nullptr;
		const HandlerWrite<dWidth, aShift> *const *dispatchWrite = nullptr;
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
