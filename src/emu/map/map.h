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
#include "emu/map/heun.h"

class cpuDevice;
class Machine;
class Console;

namespace aspace
{
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
	class BusManager;


	template <int Level, int dWidth, int aShift, endian_t type>
	class MemoryAccessSpecific
	{
		friend class AddressSpace;

		using uintx_t = typename HandlerSize<dWidth>::uintx_t;

		static constexpr uint64_t nativeBytes = 1 << dWidth;
		static constexpr uint64_t nativeMask = dWidth - aShift >= 0 ? (1u << (dWidth - aShift)) - 1 : 0;

	public:
		MemoryAccessSpecific() = default;

		inline AddressSpace &getSpace() { return *space; }
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
		void set(AddressSpace *space, const void *read, const void *write);
		// {
		// 	this->space = space;
		// 	// this->addrMask = space->getAddrMask();
		// 	this->readDispatch = (const HandlerRead<dWidth, aShift, type> *const *)(read);
		// 	this->writeDispatch = (const HandlerWrite<dWidth, aShift, type> *const *)(write);
		// }

	private:
		AddressSpace *space = nullptr;
		offs_t addrMask = 0;
		uint64_t unmapValue = 0;

		const HandlerRead<dWidth, aShift> *readDispatch = nullptr;
		const HandlerWrite<dWidth, aShift> *writeDispatch = nullptr;

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
		void set(AddressSpace *space, const void *read, const void *write);
		// {
		// 	this->space = space;
		// 	// this->addrMask = space->getAddrMask();
		// 	this->readDispatch = (const HandlerRead<dWidth, aShift, type> *const *)(read);
		// 	this->writeDispatch = (const HandlerWrite<dWidth, aShift, type> *const *)(write);
		// }

	private:
		AddressSpace *space = nullptr;
		offs_t addrMask = 0;
		uint64_t unmapValue = 0;

		const HandlerRead<dWidth, aShift> *readDispatch = nullptr;
		const HandlerWrite<dWidth, aShift> *writeDispatch = nullptr;

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

		// Unmapped value setting
		inline void setUnmapLowValue()          { unmapValue = 0; }
		inline void setUnmapHighValue()         { unmapValue = ~0ull; }
		inline void setUnmapValue(uint64_t val) { unmapValue = val; }

		// Setup initialization routines
		void prepare(Console *cty);
		void populate(Console *cty);
		void allocate(Console *cty);

		template <int Level, int dWidth, int aShift, endian_t type>
		void setMemorySpecific(MemoryAccessSpecific<Level, dWidth, aShift, type> memAccess)
		{

		}

		template <int dWidth, int aShift, endian_t type>
		void setMemoryCache(MemoryAccessCache<dWidth, aShift, type> memAccess)
		{

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

		offs_t addrMask = 0;

		uint64_t unmapValue = 0;

		// Optional main memory space
		uint8_t *memData = nullptr;
		uint64_t memSize = 0;
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
		this->addrMask = space->getAddrMask();
		this->readDispatch = (const HandlerRead<dWidth, aShift> *const *)(read);
		this->writeDispatch = (const HandlerWrite<dWidth, aShift> *const *)(write);
	}

	template <int dWidth, int aShift, endian_t type>
	void MemoryAccessCache<dWidth, aShift, type>::
	set(AddressSpace *space, const void *read, const void *write)
	{
		this->space = space;
		this->addrMask = space->getAddrMask();
		this->readDispatch = (const HandlerRead<dWidth, aShift> *const *)(read);
		this->writeDispatch = (const HandlerWrite<dWidth, aShift> *const *)(write);
	}

}

using mapSpaceType     = aspace::SpaceType;
using mapConfigEntry   = aspace::ConfigEntry;
using mapConfigList    = aspace::ConfigList;

using mapAddressConfig = aspace::AddressConfig;
using mapAddressSpace  = aspace::AddressSpace;
using mapMemoryBlock   = aspace::MemoryBlock;
using mapBusManager    = aspace::BusManager;

