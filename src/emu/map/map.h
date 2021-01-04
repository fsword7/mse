/*
 * map.h - Mapping system bus package
 *
 *  Created on: Dec 12, 2020
 *      Author: Tim Stark
 */

#pragma once

// address space types
#define AS_PROGRAM  0	// Program address space
#define AS_DATA		1	// Data address space
#define AS_IO		2	// I/O port address space


//using offs_t = uint64_t;
typedef uint64_t offs_t;

class ProcessorDevice;

namespace map
{
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

		inline uint16_t getDataWidth() const { return dataWidth; }
		inline uint16_t getDataRadix() const { return dataRadix; }
		inline uint16_t getByteWidth() const { return byteWidth; }
		inline uint16_t getAddrWidth() const { return addrWidth; }
		inline uint16_t getAddrRadix() const { return addrRadix; }
		inline int16_t  getAddrShift() const { return addrShift; }
		inline int16_t  getPageShift() const { return pageShift; }

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
		ctag_t *name = nullptr;

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

	class AddressSpaceBase
	{
	public:
		AddressSpaceBase(const AddressConfig &config) : config(config) {}
		virtual ~AddressSpaceBase() = default;

		inline const AddressConfig &getConfig() { return config; }

		inline uint16_t getDataWidth() const { return config.getDataWidth(); }
		inline uint16_t getDataRadix() const { return config.getDataRadix(); }
		inline uint16_t getAddrWidth() const { return config.getAddrWidth(); }
		inline uint16_t getAddrRadix() const { return config.getAddrRadix(); }
		inline int16_t  getAddrShift() const { return config.getAddrShift(); }
		inline int16_t  getPageShift() const { return config.getPageShift(); }

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

	protected:
		const AddressConfig &config;
	};

	class AddressSpace : public AddressSpaceBase
	{
	public:
		AddressSpace(const AddressConfig &config) : AddressSpaceBase(config) {}

		// read/write function calls
		uint8_t  read8(offs_t addr, ProcessorDevice *cpu = nullptr);
		uint16_t read16(offs_t addr, ProcessorDevice *cpu = nullptr);
		uint16_t read16u(offs_t addr, ProcessorDevice *cpu = nullptr);
		uint32_t read32(offs_t addr, ProcessorDevice *cpu = nullptr);
		uint32_t read32u(offs_t addr, ProcessorDevice *cpu = nullptr);
		uint64_t read64(offs_t addr, ProcessorDevice *cpu = nullptr);
		uint64_t read64u(offs_t addr, ProcessorDevice *cpu = nullptr);

		void write8(offs_t addr, uint8_t data, ProcessorDevice *cpu = nullptr);
		void write16(offs_t addr, uint16_t data, ProcessorDevice *cpu = nullptr);
		void write16u(offs_t addr, uint16_t data, ProcessorDevice *cpu = nullptr);
		void write32(offs_t addr, uint32_t data, ProcessorDevice *cpu = nullptr);
		void write32u(offs_t addr, uint32_t data, ProcessorDevice *cpu = nullptr);
		void write64(offs_t addr, uint64_t data, ProcessorDevice *cpu = nullptr);
		void write64u(offs_t addr, uint64_t data, ProcessorDevice *cpu = nullptr);

		// Be removed later
		void createMainMemory(offs_t length);

	private:
		// main memory access - be removed later
		uint8_t *memData = nullptr;;
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

	using BlockList = vector<MemoryBlock *>;

	class BusManager
	{
	public:
		BusManager() = default;
		~BusManager() = default;

		inline BlockList &getBlockList() { return blocks; }

		// Executed from start command
		void init(Console *cty);

	private:
		BlockList blocks;

	};
}

using mapSpaceType     = map::SpaceType;
using mapConfigEntry   = map::ConfigEntry;
using mapConfigList    = map::ConfigList;

using mapAddressConfig = map::AddressConfig;
using mapAddressSpace  = map::AddressSpace;
using mapMemoryBlock   = map::MemoryBlock;
using mapBusManager    = map::BusManager;

