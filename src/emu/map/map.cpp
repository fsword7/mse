/*
 * map.cpp - Mapping system bus package
 *
 *  Created on: Dec 12, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/dibus.h"
#include "emu/map/addrmap.h"
#include "emu/map/map.h"

using namespace aspace;

AddressConfig::AddressConfig(ctag_t *tagName, endian_t eType,
	uint16_t dWidth, uint16_t dRadix, uint16_t bWidth,
	uint16_t aWidth, uint16_t aRadix, int16_t aShift)
: name(tagName), endianType(eType),
  dataWidth(dWidth), dataRadix(dRadix), byteWidth(bWidth),
  addrWidth(aWidth), addrRadix(aRadix), addrShift(aShift)
{

}

// **********************************************************************

static ctag_t *asInfo[] = { "program", "data", "I/O port" };

AddressSpace::AddressSpace(BusManager &manager, diExternalBus &bus, int space)
: config(*bus.getAddressConfig(space)),
  device(*bus.getDevice()), manager(manager),
  space(space)
{

}

void AddressSpace::prepare(Console *cty)
{
	fmt::printf("%s: Preparing for %s address space\n", device.getDeviceName(), asInfo[space]);

	AddressList *map = new AddressList(device, space);

	unmapValue = (map->unmapValue == 0) ? 0ull : ~0ull;
	if (map->gaddrMask != 0ull)
	{
		if (map->gaddrMask & ~addrMask)
			fmt::printf("%s(%s): Can't set a global mask of %llX on a %d-bit address width (mask %llX)\n",
				device.getDeviceName(), asInfo[space], map->gaddrMask, config.getAddrWidth(), addrMask);
		addrMask = map->gaddrMask;
	}

	for (AddressEntry *entry : map->list)
	{
		fmt::printf("%s(%s): Mapping %llX-%llX mask %llX mirror %llX\n", device.getDeviceName(), asInfo[space],
			entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror);

		// Validate addresses against named region space (expandable or non-expandable)
		if (entry->regionName != nullptr)
		{
			MemoryRegion *region = manager.findRegion(entry->regionName);

			if (region == nullptr) {
				fmt::printf("%s(%s): %llX-%llX - non-existant region '%s'\n", device.getDeviceName(), asInfo[space],
					entry->addrStart, entry->addrEnd, entry->regionName);
				continue;
			}

			// Assign region space to that memory space
			entry->memData = region->getBase();
		}
	}
}

void AddressSpace::populate(Console *cty)
{
	fmt::printf("%s: Populating for %s address space\n", device.getDeviceName(), asInfo[space]);

}

void AddressSpace::allocate(Console *cty)
{
	fmt::printf("%s: Allocating memory space for %s address space\n", device.getDeviceName(), asInfo[space]);

	BlockList &blocks = manager.getBlockList();

//	memData = manager.getMemoryData();
//	memSize = manager.getMemorySize();
//
//	if (memData != nullptr && memSize > 0)
//	{
//
//	}

//	for (map::AddressEntry *entry : map->list)
//		if (map->memData != nullptr)
//			blocks.push_back(new map::MemoryBlock(config, entry->addrStart, entry->addrEnd, entry->memData));
}

// **********************************************************************

MemoryBlock::MemoryBlock(mapAddressConfig &config, offs_t sAddr, offs_t eAddr, void *base)
: config(config), addrStart(sAddr), addrEnd(eAddr),
  dataBase(reinterpret_cast<uint8_t *>(base))
{
	maxSize  = config.convertAddresstoByte(sAddr + 1 - eAddr);
	dataSize = maxSize;

	// allocate memory space as default
	if (dataBase != nullptr)
	{
		allocated.reserve(dataSize);
		memset(&allocated[0], 0, dataSize);
		dataBase = &allocated[0];
	}
}

void MemoryBlock::reserve(offs_t size)
{
	if (size > maxSize)
		size = maxSize;
	allocated.reserve(size);
	memset(&allocated[0], 0, size);
	dataBase = &allocated[0];
	dataSize = size;
}
