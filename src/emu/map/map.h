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


//using offs_t = uint64_t;
typedef uint64_t offs_t;

#include "emu/map/he.h"
#include "emu/map/heun.h"

class ProcessorDevice;
class Machine;

namespace aspace
{
	using Constructor = NamedDelegate<void (AddressList &)>;

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
	class BusManager;

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

		// Unmapped value setting
		inline void setUnmapLowValue()          { unmapValue = 0; }
		inline void setUnmapHighValue()         { unmapValue = ~0ull; }
		inline void setUnmapValue(uint64_t val) { unmapValue = val; }

		// Setup initialization routines
		void prepare(Console *cty);
		void populate(Console *cty);
		void allocate(Console *cty);

		// Virtual function calls
		virtual uint8_t  read8(offs_t addr, ProcessorDevice *cpu = nullptr) = 0;
		virtual uint16_t read16(offs_t addr, ProcessorDevice *cpu = nullptr) = 0;
		virtual uint16_t read16u(offs_t addr, ProcessorDevice *cpu = nullptr) = 0;
		virtual uint32_t read32(offs_t addr, ProcessorDevice *cpu = nullptr) = 0;
		virtual uint32_t read32u(offs_t addr, ProcessorDevice *cpu = nullptr) = 0;
		virtual uint64_t read64(offs_t addr, ProcessorDevice *cpu = nullptr) = 0;
		virtual uint64_t read64u(offs_t addr, ProcessorDevice *cpu = nullptr) = 0;

		virtual void write8(offs_t addr, uint8_t data, ProcessorDevice *cpu = nullptr) = 0;
		virtual void write16(offs_t addr, uint16_t data, ProcessorDevice *cpu = nullptr) = 0;
		virtual void write16u(offs_t addr, uint16_t data, ProcessorDevice *cpu = nullptr) = 0;
		virtual void write32(offs_t addr, uint32_t data, ProcessorDevice *cpu = nullptr) = 0;
		virtual void write32u(offs_t addr, uint32_t data, ProcessorDevice *cpu = nullptr) = 0;
		virtual void write64(offs_t addr, uint64_t data, ProcessorDevice *cpu = nullptr) = 0;
		virtual void write64u(offs_t addr, uint64_t data, ProcessorDevice *cpu = nullptr) = 0;
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
}

using mapSpaceType     = aspace::SpaceType;
using mapConfigEntry   = aspace::ConfigEntry;
using mapConfigList    = aspace::ConfigList;

using mapAddressConfig = aspace::AddressConfig;
using mapAddressSpace  = aspace::AddressSpace;
using mapMemoryBlock   = aspace::MemoryBlock;
using mapBusManager    = aspace::BusManager;

