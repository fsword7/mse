/*
 * map.h - Mapping system bus package
 *
 *  Created on: Dec 12, 2020
 *      Author: Tim Stark
 */

#pragma once

#include <type_traits>

namespace aspace
{
	class AddressSpace;
	class AddressList;
}

// address space types
#define AS_PROGRAM  0	// Program address space
#define AS_DATA		1	// Data address space
#define AS_IOPORT	2	// I/O port address space


////using offs_t = uint64_t;
//typedef uint64_t offs_t;

#include "emu/map/he.h"
// #include "emu/map/heun.h"
// #include "emu/map/hedr.h"
// #include "emu/map/hedw.h"
// #include "emu/map/hea.h"
// #include "emu/map/hem.h"

class cpuDevice;
class Machine;
class Console;
class Device;
class diExternalBus;

namespace aspace
{
	template <int dWidth, int aShift> class HandlerReadUnmapped;
	template <int dWidth, int aShift> class HandlerWriteUnmapped;
	template <int dWidth, int aShift> class HandlerReadNop;
	template <int dWidth, int aShift> class HandlerWriteNop;

	using Constructor = NamedDelegate<void (AddressList &)>;

	using read8d_t  = DeviceDelegate<uint8_t(AddressList &, offs_t, uint8_t)>;
	using read16d_t = DeviceDelegate<uint16_t(AddressList &, offs_t, uint16_t)>;
	using read32d_t = DeviceDelegate<uint32_t(AddressList &, offs_t, uint32_t)>;
	using read64d_t = DeviceDelegate<uint64_t(AddressList &, offs_t, uint64_t)>;

	using write8d_t  = DeviceDelegate<uint8_t(AddressList &, offs_t, uint8_t)>;
	using write16d_t = DeviceDelegate<uint16_t(AddressList &, offs_t, uint16_t)>;
	using write32d_t = DeviceDelegate<uint32_t(AddressList &, offs_t, uint32_t)>;
	using write64d_t = DeviceDelegate<uint64_t(AddressList &, offs_t, uint64_t)>;

	enum SpaceType
	{
		asProgram = 0,	// Program address space
		asData,			// Data address space
		asPort			// I/O port address space
	};
	
	class AddressConfig
	{
	public:
		AddressConfig() = default;
		AddressConfig(ctag_t *name, endian_t eType,
			uint16_t dWidth, uint16_t dRadix, uint16_t bWidth,
			uint16_t aWidth, uint16_t aRadix, int16_t aShift);
		~AddressConfig() = default;

		// Getter function calls
		inline ctag_t *getName() const { return name; }

		inline endian_t getEndianType() const { return endianType; }
		inline uint16_t getDataWidth()  const { return dataWidth; }
		inline uint16_t getDataRadix()  const { return dataRadix; }
		inline uint16_t getByteWidth()  const { return byteWidth; }
		inline uint16_t getAddrWidth()  const { return addrWidth; }
		inline uint16_t getAddrRadix()  const { return addrRadix; }
		inline int16_t  getAddrShift()  const { return addrShift; }
		inline int16_t  getPageShift()  const { return pageShift; }

		inline offs_t convertAddresstoByte(offs_t address) const
		{
			return (addrShift < 0)
				? (address << -addrShift)
				: (address >> addrShift);
		}

		inline offs_t convertBytetoAddress(offs_t address) const
		{
			return (addrShift > 0)
				? (address << addrShift)
				: (address >> -addrShift);
		}

		inline offs_t convertAddresstoByteEnd(offs_t address) const
		{
			return (addrShift < 0)
				? ((address << -addrShift) | ((1 << -addrShift) - 1))
				: (address >> addrShift);
		}

		inline offs_t convertBytetoAddressEnd(offs_t address) const
		{
			return (addrShift > 0)
				? ((address << addrShift) | ((1 << addrShift) - 1))
				: (address >> -addrShift);
		}


	private:
		ctag_t    *name = nullptr;

		endian_t endianType = LittleEndian;
		uint16_t dataWidth  = 0;
		uint16_t dataRadix  = 0;
		uint16_t byteWidth  = 0;
		uint16_t addrWidth  = 0;
		uint16_t addrRadix  = 0;
		int16_t  addrShift  = 0;
		int16_t  pageShift  = 0;
	};

	struct ConfigEntry
	{
		const SpaceType      type;
		const AddressConfig *config;
	};

	using ConfigList = vector<ConfigEntry>;
	using cAddressConfig = const AddressConfig;

	class AddressList;
	class AddressSpace;
	class AddressEntry;
	class BusManager;

	// class MemoryAccessHeader
	// {

	// }

	inline offs_t convertAddressToByte(offs_t addr, int addrShift) { return (addrShift >= 0) ? addr << addrShift : addr >> -addrShift; }

	template <int dWidth, int aShift, endian_t eType, int tWidth, bool aligned, typename T>
	typename HandlerSize<tWidth>::uintx_t readMemory(offs_t addr, typename HandlerSize<tWidth>::uintx_t mask, cpuDevice *cpu, T rop)
	{
		using targetType = typename HandlerSize<tWidth>::uintx_t;
		using nativeType = typename HandlerSize<dWidth>::uintx_t;

		constexpr uint64_t targetBytes = 1 << tWidth;
		constexpr uint64_t targetBits = 8 * targetBytes;
		constexpr uint64_t nativeBytes = 1 << dWidth;
		constexpr uint64_t nativeBits = 8 * nativeBytes;
		constexpr uint64_t nativeStep = aShift >= 0 ? nativeBytes << aShift : nativeBytes >> -aShift;
		constexpr uint64_t nativeMask = dWidth - aShift >= 0 ? makeBitmask<uint64_t>(dWidth - aShift) : 0;

		if (nativeBytes == targetBytes && (aligned || (addr & nativeMask) == 0))
			return rop(addr & ~nativeMask, mask, cpu);

		if (nativeBytes > targetBytes)
		{
			uint32_t offBits = (convertAddressToByte(addr, aShift) * 8) & (nativeBytes - (aligned ? targetBytes : 1));
			if (aligned || (offBits + targetBits <= nativeBits))
			{
				if (eType != LittleEndian)
					offBits = nativeBits - targetBits - offBits;
				return rop(addr & nativeMask, (nativeType)mask << offBits, cpu) >> offBits;
			}
		}

		uint32_t offBits = (convertAddressToByte(addr, aShift) * 8) & (nativeBytes - 1);
		addr &= ~nativeMask;

		if (nativeBytes >= targetBytes)
		{
			if (eType == LittleEndian)
			{
				// Little-endian unaligned read access
				targetType result = 0;
				nativeType curMask = (nativeType)mask << offBits;
				if (curMask != 0)
					result = rop(addr, curMask, cpu) >> offBits;
				offBits = nativeBits - offBits;
				curMask = mask >> offBits;
				if (curMask != 0)
					result |= rop(addr + nativeStep, curMask, cpu) << offBits;
				return result;
			}
			else
			{
				// Big-endian unaligned read access
				constexpr uint32_t nativeShift =
					((nativeBits > targetBits) ? (nativeBits - targetBits) : 0);
				targetType result = 0;
				nativeType leftMask = (nativeType)mask << nativeShift;
				nativeType curMask = leftMask >> offBits;

				if (curMask != 0)
					result = rop(addr, curMask, cpu) << offBits;
				offBits = nativeBits - offBits;
				curMask = leftMask << offBits;
				if (curMask != 0)
					result |= rop(addr + nativeStep, curMask, cpu) >> offBits;
				return result >> nativeShift;
			}
		}
		else
		{
			constexpr uint32_t maxSplits = targetBytes / nativeBytes - 1;
			targetType result = 0;

			if (eType == LittleEndian)
			{
				nativeType curMask = mask << offBits;
				if (curMask != 0)
					result = rop(addr, curMask, cpu) >> offBits;
				// printf("rop: Addr: %X Off: %d Mask %X => %X\n", addr, offBits, curMask, result);

				offBits = nativeBits - offBits;
				for (uint32_t idx = 0; idx < maxSplits; idx++)
				{
					addr += nativeStep;
					curMask = mask >> offBits;
					if (curMask != 0)
						result |= (targetType)rop(addr, curMask, cpu) << offBits;
					// printf("rop: Addr: %X Off: %d Mask %X => %X\n", addr, offBits, curMask, result);
					offBits += nativeBits;
				}

				if (!aligned && offBits < targetBits)
				{
					curMask = mask >> offBits;
					if (curMask != 0)
						result |= (targetType)rop(addr + nativeStep, curMask, cpu) << offBits;
					// printf("rop: Addr: $X Off: %d Mask %X => %X\n", addr + nativeStep, offBits, curMask, result);
				}
			}
			else if (eType == BigEndian)
			{
				offBits = targetBits - (nativeBits - offBits);
				nativeType curMask = mask >> offBits;
				if (curMask != 0)
					result = rop(addr, curMask, cpu) << offBits;

				for (uint32_t idx = 0; idx < maxSplits; idx++)
				{
					offBits -= nativeBits;
					addr += nativeStep;
					curMask = mask >> offBits;
					if (curMask != 0)
						result |= (targetType)rop(addr, curMask, cpu) << offBits;
				}

				if (!aligned && offBits != 0)
				{
					offBits = nativeBits - offBits;
					curMask = mask << offBits;
					if (curMask != 0)
						result |= (targetType)rop(addr + nativeStep, curMask, cpu) >> offBits;
				}					
			}

			return result;
		}
	}

	template <int dWidth, int aShift, endian_t eType, int tWidth, bool aligned, typename T>
	void writeMemory(offs_t addr, typename HandlerSize<tWidth>::uintx_t data, typename HandlerSize<tWidth>::uintx_t mask, cpuDevice *cpu, T wop)
	{
		using targetType = typename HandlerSize<tWidth>::uintx_t;
		using nativeType = typename HandlerSize<dWidth>::uintx_t;

		constexpr uint64_t targetBytes = 1 << tWidth;
		constexpr uint64_t targetBits = 8 * targetBytes;
		constexpr uint64_t nativeBytes = 1 << dWidth;
		constexpr uint64_t nativeBits = 8 * nativeBytes;
		constexpr uint64_t nativeStep = aShift >= 0 ? nativeBytes << aShift : nativeBytes >> -aShift;
		constexpr uint64_t nativeMask = dWidth - aShift >= 0 ? makeBitmask<uint64_t>(dWidth + aShift) : 0;

		if (nativeBytes == targetBytes && (aligned || (addr & nativeMask) == 0))
			return wop(addr & ~nativeMask, data, mask, cpu);

		if (nativeBytes > targetBytes)
		{
			uint32_t offBits = (convertAddressToByte(addr, aShift) * 8) & (nativeBytes - (aligned ? targetBytes : 1));
			if (aligned || (offBits + targetBits <= nativeBits))
			{
				if (eType != LittleEndian)
					offBits = nativeBits - targetBits - offBits;
				return wop(addr & nativeMask, (nativeType)data << offBits, (nativeType)mask << offBits, cpu);
			}
		}

		uint32_t offBits = (convertAddressToByte(addr, aShift) * 8) & (nativeBytes - 1);
		addr &= ~nativeMask;

		if (nativeBytes >= targetBytes)
		{
			if (eType == LittleEndian)
			{
				// Little-endian unaligned read access
				nativeType curMask = (nativeType)mask << offBits;
				if (curMask != 0)
					wop(addr, (nativeType)data << offBits, curMask, cpu);
				offBits = nativeBits - offBits;
				curMask = mask >> offBits;
				if (curMask != 0)
					wop(addr + nativeStep, data >> offBits, curMask, cpu);
			}
			else
			{
				// Big-endian unaligned read access
				constexpr uint32_t nativeShift =
					((nativeBits > targetBits) ? (nativeBits - targetBits) : 0);
				nativeType leftData = (nativeType)data << nativeShift;
				nativeType leftMask = (nativeType)mask << nativeShift;
				nativeType curMask = leftMask >> offBits;

				if (curMask != 0)
					wop(addr, leftData >> offBits, curMask, cpu);
				offBits = nativeBits - offBits;
				curMask = leftMask << offBits;
				if (curMask != 0)
					wop(addr + nativeStep, leftData << offBits, curMask, cpu);
			}
		}
		else
		{
			constexpr uint32_t maxSplits = targetBytes / nativeBytes - 1;

			if (eType == LittleEndian)
			{
				nativeType curMask = mask << offBits;
				if (curMask != 0)
					wop(addr, data << offBits, curMask, cpu);

				offBits = nativeBits - offBits;
				for (uint32_t idx = 0; idx < maxSplits; idx++)
				{
					addr += nativeStep;
					curMask = mask >> offBits;
					if (curMask != 0)
						wop(addr, data >> offBits, curMask, cpu);
					offBits += nativeBits;
				}

				if (!aligned && offBits < targetBits)
				{
					curMask = mask >> offBits;
					if (curMask != 0)
						wop(addr + nativeStep, data >> offBits, curMask, cpu);
				}
			}
			else if (eType == BigEndian)
			{
				offBits = targetBits - (nativeBits - offBits);
				nativeType curMask = mask >> offBits;
				if (curMask != 0)
					wop(addr, data >> offBits, curMask, cpu);

				for (uint32_t idx = 0; idx < maxSplits; idx++)
				{
					offBits -= nativeBits;
					addr += nativeStep;
					curMask = mask >> offBits;
					if (curMask != 0)
						wop(addr, data >> offBits, curMask, cpu);
				}

				if (!aligned && offBits != 0)
				{
					offBits = nativeBits - offBits;
					curMask = mask << offBits;
					if (curMask != 0)
						wop(addr + nativeStep, data << offBits, curMask, cpu);
				}					
			}
		}
	}

	template <int Level, int dWidth, int aShift, endian_t eType>
	class MemoryAccessSpecific
	{
		friend class AddressSpace;

		using uintx_t = typename HandlerSize<dWidth>::uintx_t;
		using nativeType = typename HandlerSize<dWidth>::uintx_t;

		static constexpr int      pageBits = determineDispatchLowBits(Level, dWidth, aShift);
		static constexpr uint64_t nativeBytes = 1 << dWidth;
		static constexpr uint64_t nativeMask = dWidth - aShift >= 0 ? (1u << (dWidth - aShift)) - 1 : 0;

	public:
		MemoryAccessSpecific() = default;

		inline AddressSpace &getSpace() { return *space; }

	private:

		inline convertAddresToByte(offs_t addr, int addrShift) { return (addrShift >= 0) ? addr << addrShift : addr >> -addrShift; }

		inline uintx_t readNative(offs_t addr, cpuDevice *cpu)
		{
			// printf("Dispatch 1M Offset: (%X & %X) >> %d => %X\n", addr, addrMask,
			// 	pageBits, (addr & addrMask) >> pageBits);
			return readDispatch[(addr & addrMask) >> pageBits]->read(addr, cpu);
		}

		inline uintx_t readNative(offs_t addr, uintx_t mask, cpuDevice *cpu)
		{
			// printf("Dispatch 1M Offset: (%X & %X) >> %d => %X\n", addr, addrMask,
			// 	pageBits, (addr & addrMask) >> pageBits);
			return readDispatch[(addr & addrMask) >> pageBits]->read(addr, mask, cpu);
		}

		inline void writeNative(offs_t addr, uintx_t data, cpuDevice *cpu)
		{
			writeDispatch[(addr & addrMask) >> pageBits]->write(addr, data, cpu);
		}

		inline void writeNative(offs_t addr, uintx_t data, uintx_t mask, cpuDevice *cpu)
		{
			writeDispatch[(addr & addrMask) >> pageBits]->write(addr, data, mask, cpu);
		}


	public:
		uint8_t read8(offs_t addr, cpuDevice *cpu)
		{
			// if (addr < memSize)
			// 	return memData[addr];
			if (dWidth == 0)
				return readNative(addr, cpu);
			else
				return readMemory<dWidth, aShift, eType, 0, true>(addr, 0xFF, cpu,
					[this](offs_t addr, nativeType mask, cpuDevice *cpu)
						-> nativeType { return readNative(addr, mask, cpu); });
			// return unmapValue;
		}

		uint16_t read16(offs_t addr, cpuDevice *cpu)
		{
			// if (addr < memSize) {
			// 	uint8_t *ptr = memData + (addr & ~0x1);
			// 	return *((uint16_t *)ptr);
			// }
			if (dWidth == 1)
				return readNative(addr, cpu);
			else
				return readMemory<dWidth, aShift, eType, 1, true>(addr, 0xFFFF, cpu,
					[this](offs_t addr, nativeType mask, cpuDevice *cpu)
						-> nativeType { return readNative(addr, mask, cpu); });
			// return unmapValue;
		}

		uint16_t read16u(offs_t addr, cpuDevice *cpu)
		{
			// if (addr < memSize) {
			// 	uint8_t *ptr = memData + addr;
			// 	return *((uint16_t *)ptr);
			// }
			return readMemory<dWidth, aShift, eType, 1, false>(addr, 0xFFFF, cpu,
				[this](offs_t addr, nativeType mask, cpuDevice *cpu)
					-> nativeType { return readNative(addr, mask, cpu); });
			// return unmapValue;
		}

		uint32_t read32(offs_t addr, cpuDevice *cpu)
		{
			// if (addr < memSize) {
			// 	uint8_t *ptr = memData + (addr & ~0x3);
			// 	return *((uint32_t *)ptr);
			// }
			if (dWidth == 2)
				return readNative(addr, cpu);
			else
				return readMemory<dWidth, aShift, eType, 2, true>(addr, 0xFFFFFFFF, cpu,
					[this](offs_t addr, nativeType mask, cpuDevice *cpu)
						-> nativeType { return readNative(addr, mask, cpu); });
			// return unmapValue;
		}

		uint32_t read32u(offs_t addr, cpuDevice *cpu)
		{
			// if (addr < memSize) {
			// 	uint8_t *ptr = memData + addr;
			// 	return *((uint32_t *)ptr);
			// }
			return readMemory<dWidth, aShift, eType, 2, false>(addr, 0xFFFFFFFF, cpu,
				[this](offs_t addr, nativeType mask, cpuDevice *cpu)
					-> nativeType { return readNative(addr, mask, cpu); });
			// return unmapValue;
		}

		uint64_t read64(offs_t addr, cpuDevice *cpu)
		{
			// if (addr < memSize) {
			// 	uint8_t *ptr = memData + (addr & ~0x7);
			// 	return *((uint64_t *)ptr);
			// }
			if (dWidth == 3)
				return readNative(addr, cpu);
			else
				return readMemory<dWidth, aShift, eType, 3, true>(addr, 0xFFFFFFFF'FFFFFFFull, cpu,
					[this](offs_t addr, nativeType mask, cpuDevice *cpu)
						-> nativeType { return readNative(addr, mask, cpu); });
			// return unmapValue;
		}

		uint64_t read64u(offs_t addr, cpuDevice *cpu)
		{
			// if (addr < memSize) {
			// 	uint8_t *ptr = memData + addr;
			// 	return *((uint64_t *)ptr);
			// }
			return readMemory<dWidth, aShift, eType, 3, false>(addr, 0xFFFFFFFF'FFFFFFFFull, cpu,
				[this](offs_t addr, nativeType mask, cpuDevice *cpu)
					-> nativeType { return readNative(addr, mask, cpu); });
			// return unmapValue;
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
			// if (addr < memSize)
			// 	memData[addr] = data;
			if (dWidth == 0)
				writeNative(addr, data, cpu);
			else
				writeMemory<dWidth, aShift, eType, 0, true>(addr, data, 0xFF, cpu,
					[this](offs_t addr, nativeType data, nativeType mask, cpuDevice *cpu)
						{ writeNative(addr, data, mask, cpu); });
		}

		void write16(offs_t addr, uint16_t data, cpuDevice *cpu)
		{
			// if (addr < memSize) {
			// 	uint8_t *ptr = memData + (addr & ~0x1);
			// 	*((uint16_t *)ptr) = data;
			// }
			if (dWidth == 1)
				writeNative(addr, data, cpu);
			else
				writeMemory<dWidth, aShift, eType, 1, true>(addr, data, 0xFFFF, cpu,
					[this](offs_t addr, nativeType data, nativeType mask, cpuDevice *cpu)
						{ writeNative(addr, data, mask, cpu); });
		}

		void write16u(offs_t addr, uint16_t data, cpuDevice *cpu)
		{
			// if (addr < memSize) {
			// 	uint8_t *ptr = memData + addr;
			// 	*((uint16_t *)ptr) = data;
			// }
			writeMemory<dWidth, aShift, eType, 1, false>(addr, data, 0xFFFF, cpu,
				[this](offs_t addr, nativeType data, nativeType mask, cpuDevice *cpu)
					{ writeNative(addr, data, mask, cpu); });
		}

		void write32(offs_t addr, uint32_t data, cpuDevice *cpu)
		{
			// if (addr < memSize) {
			// 	uint8_t *ptr = memData + (addr & ~0x2);
			// 	*((uint32_t *)ptr) = data;
			// }
			if (dWidth == 2)
				writeNative(addr, data, cpu);
			else
				writeMemory<dWidth, aShift, eType, 2, true>(addr, data, 0xFFFFFFFF, cpu,
					[this](offs_t addr, nativeType data, nativeType mask, cpuDevice *cpu)
						{ writeNative(addr, data, mask, cpu); });
		}

		void write32u(offs_t addr, uint32_t data, cpuDevice *cpu)
		{
			// if (addr < memSize) {
			// 	uint8_t *ptr = memData + addr;
			// 	*((uint32_t *)ptr) = data;
			// }
			writeMemory<dWidth, aShift, eType, 2, false>(addr, data, 0xFFFFFFFF, cpu,
				[this](offs_t addr, nativeType data, nativeType mask, cpuDevice *cpu)
					{ writeNative(addr, data, mask, cpu); });
		}

		void write64(offs_t addr, uint64_t data, cpuDevice *cpu)
		{
			// if (addr < memSize) {
			// 	uint8_t *ptr = memData + (addr & ~0x3);
			// 	*((uint64_t *)ptr) = data;
			// }
			if (dWidth == 3)
				writeNative(addr, data, cpu);
			else
				writeMemory<dWidth, aShift, eType, 3, true>(addr, data, 0xFFFFFFFF'FFFFFFFFull, cpu,
					[this](offs_t addr, nativeType data, nativeType mask, cpuDevice *cpu)
						{ writeNative(addr, data, mask, cpu); });
		}

		void write64u(offs_t addr, uint64_t data, cpuDevice *cpu)
		{
			// if (addr < memSize) {
			// 	uint8_t *ptr = memData + addr;
			// 	*((uint64_t *)ptr) = data;
			// }
			writeMemory<dWidth, aShift, eType, 3, false>(addr, data, 0xFFFFFFFF'FFFFFFFFull, cpu,
				[this](offs_t addr, nativeType data, nativeType mask, cpuDevice *cpu)
					{ writeNative(addr, data, mask, cpu); });
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
		void set(AddressSpace *space, const void *read, const void *write);

	private:
		AddressSpace *space = nullptr;
		offs_t addrMask = 0;
		uint64_t unmapValue = 0;

		const HandlerRead<dWidth, aShift> *const *readDispatch = nullptr;
		const HandlerWrite<dWidth, aShift> *const *writeDispatch = nullptr;

		offs_t   memSize = 0;
		uint8_t *memData = nullptr;
	};

	template <int dWidth, int aShift, endian_t type>
	class MemoryAccessCache
	{
		friend class AddressSpace;

		using uintx_t = typename HandlerSize<dWidth>::uintx_t;

		static constexpr uint64_t nativeBytes = 1 << dWidth;
		static constexpr uint64_t nativeMask = dWidth - aShift >= 0 ? (1u << (dWidth - aShift)) - 1 : 0;

	public:
		MemoryAccessCache() = default;

		inline AddressSpace &getSpace() { return *space; }
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
		void set(AddressSpace *space, const void *read, const void *write);

	private:
		AddressSpace *space = nullptr;
		offs_t addrMask = 0;
		uint64_t unmapValue = 0;

		const HandlerRead<dWidth, aShift> *const *readDispatch = nullptr;
		const HandlerWrite<dWidth, aShift> *const *writeDispatch = nullptr;

		offs_t   memSize = 0;
		uint8_t *memData = nullptr;
	};

	template <int highBits, int dWidth, int aShift, endian_t type>
	struct MemoryAccess
	{
		static constexpr int Level = determineDispatchLevel(highBits);

		using specific = MemoryAccessSpecific<Level, dWidth, aShift, type>;
		using cache = MemoryAccessCache<dWidth, aShift, type>;
	};

	class AddressSpace
	{
	public:
		enum accessType { accRead, accWrite };

		AddressSpace(BusManager &manager, diExternalBus &bus, int space);

		virtual ~AddressSpace() = default;

		inline const AddressConfig &getConfig() { return config; }

		inline uint16_t getDataWidth() const { return config.getDataWidth(); }
		inline uint16_t getDataRadix() const { return config.getDataRadix(); }
		inline uint16_t getAddrWidth() const { return config.getAddrWidth(); }
		inline uint16_t getAddrRadix() const { return config.getAddrRadix(); }
		inline int16_t  getAddrShift() const { return config.getAddrShift(); }
		inline int16_t  getPageShift() const { return config.getPageShift(); }
		inline offs_t   getAddrMask() const  { return addrMask; }
		inline uint64_t getUnmapped() const  { return unmapValue; }

		// inline HandlerEntry *getUnmappedRead() const { return unmapRead; }
		// inline HandlerEntry *getUnmappedWrite() const { return unmapWrite; }
		
		template <int dWidth, int aShift>
		HandlerReadUnmapped<dWidth, aShift> *getUnmappedRead() const
		{
			return static_cast<HandlerReadUnmapped<dWidth, aShift> *>(unmapRead);
		}

		template <int dWidth, int aShift>
		HandlerWriteUnmapped<dWidth, aShift> *getUnmappedWrite() const
		{
			return static_cast<HandlerWriteUnmapped<dWidth, aShift> *>(unmapWrite);
		}

		// Unmapped value setting
		inline void setUnmapLowValue()          { unmapValue = 0; }
		inline void setUnmapHighValue()         { unmapValue = ~0ull; }
		inline void setUnmapValue(uint64_t val) { unmapValue = val; }

		// Setup initialization routines
		void prepare(Console *cty);
		void populate(Console *cty);
		void allocate(Console *cty);
		
		void populateEntry(const AddressEntry *entry, accessType rwType);

		virtual void setMemorySpace(offs_t addrStart, offs_t addrEnd, offs_t addrMirror, uint8_t *data, accessType rwType) = 0;

		virtual std::pair<const void *, const void *> getSpecificDispatches() = 0;
		virtual std::pair<const void *, const void *> getCacheDispatches() = 0;

		template <int Level, int dWidth, int aShift, endian_t type>
		void setMemorySpecific(MemoryAccessSpecific<Level, dWidth, aShift, type> &memAccess)
		{
			std::pair<const void *, const void *> rw = getSpecificDispatches();

			memAccess.set(this, rw.first, rw.second);
		}

		template <int dWidth, int aShift, endian_t type>
		void setMemoryCache(MemoryAccessCache<dWidth, aShift, type> &memAccess)
		{
			std::pair<const void *, const void *> rw = getCacheDispatches();

			memAccess.set(this, rw.first, rw.second);
		}

		// Virtual function calls
		virtual uint8_t  read8(offs_t addr, cpuDevice *cpu = nullptr) = 0;
		virtual uint16_t read16(offs_t addr, cpuDevice *cpu = nullptr) = 0;
		virtual uint16_t read16u(offs_t addr, cpuDevice *cpu = nullptr) = 0;
		virtual uint32_t read32(offs_t addr, cpuDevice *cpu = nullptr) = 0;
		virtual uint32_t read32u(offs_t addr, cpuDevice *cpu = nullptr) = 0;
		virtual uint64_t read64(offs_t addr, cpuDevice *cpu = nullptr) = 0;
		virtual uint64_t read64u(offs_t addr, cpuDevice *cpu = nullptr) = 0;
		virtual void     readBlock(offs_t addr, uint8_t *data, uint64_t size) = 0;

		virtual void write8(offs_t addr, uint8_t data, cpuDevice *cpu = nullptr) = 0;
		virtual void write16(offs_t addr, uint16_t data, cpuDevice *cpu = nullptr) = 0;
		virtual void write16u(offs_t addr, uint16_t data, cpuDevice *cpu = nullptr) = 0;
		virtual void write32(offs_t addr, uint32_t data, cpuDevice *cpu = nullptr) = 0;
		virtual void write32u(offs_t addr, uint32_t data, cpuDevice *cpu = nullptr) = 0;
		virtual void write64(offs_t addr, uint64_t data, cpuDevice *cpu = nullptr) = 0;
		virtual void write64u(offs_t addr, uint64_t data, cpuDevice *cpu = nullptr) = 0;
		virtual void writeBlock(offs_t addr, uint8_t *data, uint64_t size) = 0;

	protected:
		cAddressConfig &config;
		Device         &device;
		BusManager     &manager;
		int             space;

		AddressList    *map = nullptr;

		offs_t   addrMask = 0;
		uint64_t unmapValue = 0;

		// Optional main memory space
		uint8_t *memData = nullptr;
		uint64_t memSize = 0;

		// Unmapped/no-operation read/write accesses
		HandlerEntry *unmapRead = nullptr;
		HandlerEntry *unmapWrite = nullptr;
		HandlerEntry *nopRead = nullptr;
		HandlerEntry *nopWrite = nullptr;
	};

	class MemoryBlock
	{
	public:
		MemoryBlock(AddressConfig &config, offs_t sAddr, offs_t eAddr, void *base = nullptr);
		~MemoryBlock() = default;

		inline offs_t getStartAddress() const { return addrStart; }
		inline offs_t getEndAddress() const   { return addrEnd; }
		inline uint8_t *getData() const       { return dataBase; }
		inline offs_t getSize() const         { return dataSize; }

		void reserve(offs_t size);

	private:
		AddressConfig &config;

		offs_t   addrStart;
		offs_t   addrEnd;
		uint8_t *dataBase;
		offs_t   dataSize;
		offs_t   maxSize;

		vector<uint8_t> allocated;
	};

	class MemoryRegion
	{
	public:
		MemoryRegion(Machine *sys, cstag_t &name, uint64_t size, int width, endian_t type)
		: system(sys), name(name), data(size), width(width), type(type)
		{
			assert(width == 8 || width == 16 || width == 32 || width == 64);
		}

		inline Machine  *getMachine() const  { return system; }
//		inline ctag_t   *getName() const     { return name.c_str(); }
		inline cstag_t  &getName() const     { return name; }
		inline uint64_t  getSize() const     { return data.size(); }
		inline endian_t  getEndian() const   { return type; }
		inline int       getBitWidth() const { return width; }

		uint8_t *getBase() { return (data.size() > 0) ? &data[0] : nullptr; }
		uint8_t *getEnd()  { return getBase() + data.size(); }

		// Get data access
		uint8_t  &getAccess8(offs_t off = 0) { return data[off]; }
		uint16_t &getAccess16(offs_t off = 0) { return reinterpret_cast<uint16_t *>(getBase())[off]; }
		uint32_t &getAccess32(offs_t off = 0) { return reinterpret_cast<uint32_t *>(getBase())[off]; }
		uint64_t &getAccess64(offs_t off = 0) { return reinterpret_cast<uint64_t *>(getBase())[off]; }

	private:
		Machine        *system = nullptr;
		cstag_t         name;
		vector<uint8_t> data;
		int             width;
		endian_t        type;
	};

	using BlockList =  vector<MemoryBlock *>;
	using RegionList = std::map<string, MemoryRegion *>;

	class BusManager
	{
	public:
		BusManager(Machine *sys) : system(sys) { blocks.clear();  regions.clear(); }
		~BusManager() = default;

		inline Machine *getMachine()       { return system; }
		inline BlockList &getBlockList()   { return blocks; }
		inline RegionList &getRegionList() { return regions; }

		inline uint8_t *getMemoryData() const { return memData; }
		inline uint64_t getMemorySize() const { return memSize; }

		void allocateMainMemory(uint64_t size, uint8_t value = 0);
		MemoryRegion *allocateRegion(cstag_t &name, uint64_t size, int width, endian_t type);
		void releaseRegion(cstag_t &name);
		MemoryRegion *findRegion(cstag_t &name);

		// Executed from start command
		void init(Console *cty);
		void allocate(Console *cty, diExternalBus &bus);
		uint8_t *allocateMemory(AddressSpace *space, offs_t start, offs_t end, offs_t size);

	private:
		Machine *system = nullptr;
		BlockList blocks;
		RegionList regions;

		// Main memory space allocation
		uint8_t  *memData = nullptr;
		uint64_t  memSize = 0;
	};


	template <int Level, int dWidth, int aShift, endian_t type>
	void MemoryAccessSpecific<Level, dWidth, aShift, type>::
	set(AddressSpace *space, const void *read, const void *write)
	{
		this->space = space;
		addrMask = space->getAddrMask();
		readDispatch = (const HandlerRead<dWidth, aShift> *const *)(read);
		writeDispatch = (const HandlerWrite<dWidth, aShift> *const *)(write);
	}

	template <int dWidth, int aShift, endian_t type>
	void MemoryAccessCache<dWidth, aShift, type>::
	set(AddressSpace *space, const void *read, const void *write)
	{
		// this->space = space;
		// addrMask = space->getAddrMask();
		// readDispatch = (const HandlerRead<dWidth, aShift> *const *)(read);
		// writeDispatch = (const HandlerWrite<dWidth, aShift> *const *)(write);
	}

}

#include "emu/map/heun.h"
#include "emu/map/hedr.h"
#include "emu/map/hedw.h"
#include "emu/map/hea.h"
#include "emu/map/hem.h"
#include "emu/map/hep.h"

using mapSpaceType     = aspace::SpaceType;
using mapConfigEntry   = aspace::ConfigEntry;
using mapConfigList    = aspace::ConfigList;

using mapAddressConfig = aspace::AddressConfig;
using mapAddressSpace  = aspace::AddressSpace;
using mapMemoryBlock   = aspace::MemoryBlock;
using mapMemoryRegion  = aspace::MemoryRegion;
using mapBusManager    = aspace::BusManager;

