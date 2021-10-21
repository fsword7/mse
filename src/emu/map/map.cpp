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
	addrMask = makeBitmask<offs_t>(config.getAddrWidth());
}

void AddressSpace::prepare(Console *cty)
{
	fmt::printf("%s: Preparing for %s address space\n", device.getDeviceName(), asInfo[space]);

	assert(map == nullptr);
	map = new AddressList(device, space);

	unmapValue = map->unmapValue;
	if (map->gaddrMask != 0ull)
	{
		if (map->gaddrMask & ~addrMask)
			fmt::printf("%s(%s): Can't set a global mask of %llX on a %d-bit address width (mask %llX)\n",
				device.getDeviceName(), asInfo[space], map->gaddrMask, config.getAddrWidth(), addrMask);
		addrMask = map->gaddrMask;
	}

	// MemoryRegion *rgnDevice = nullptr;
	// offs_t        rgmSize   = (rgnDevice != nullptr) ? rgnDevice->getSize() : 0;

	for (AddressEntry *entry : map->list)
	{
		fmt::printf("%s(%s): Mapping %llX-%llX mask %llX mirror %llX\n", device.getDeviceName(), asInfo[space],
			entry->addrStart, entry->addrEnd, entry->addrMask, entry->addrMirror);

		if (entry->shareName != nullptr)
		{
			MemoryShare *share = manager.findShare(entry->shareName);

			if (share == nullptr)
			{
				// Allocate sharing named memory space
				size_t length = config.convertAddressToByte(entry->addrEnd+1 - entry->addrStart);

				fmt::printf("%s(%s): Creating share '%s' of length %X\n", device.getDeviceName(),
					asInfo[space], entry->shareName, length);
				share = manager.allocateShare(device, entry->shareName, length,
					config.getDataWidth(), config.getEndianType());
				entry->memData = (uint8_t *)share->getData();
			}
			else
			{
				// Assign sharing memory space from memory share list
				size_t length = config.convertAddressToByte(entry->addrEnd+1 - entry->addrStart);
				cstag_t result = share->compare(length, config.getDataWidth(), config.getEndianType());
				if (!result.empty())
					fmt::printf("%s(%s): %s\n", device.getDeviceName(), asInfo[space], result);
				else
					entry->memData = (uint8_t *)share->getData();
			}
		}

		// Validate addresses against named region space (expandable or non-expandable)
		if (entry->regionName != nullptr)
		{
			MemoryRegion *region = manager.findRegion(entry->regionName);

			if (region == nullptr) {
				fmt::printf("%s(%s): %llX-%llX - non-existant region '%s'\n", device.getDeviceName(), asInfo[space],
					entry->addrStart, entry->addrEnd, entry->regionName);
				continue;
			}

			// Determine ending address for expandable memory space
			if (region->getSize() < (entry->addrEnd - entry->addrStart + 1)) {
				fmt::printf("%s(%s): %llX-%llX - expandable range up to %llX\n", device.getDeviceName(), asInfo[space],
					entry->addrStart, (entry->addrStart + region->getSize() - 1), entry->addrEnd);
				// New ending address for desired memory length
				entry->addrEnd = entry->addrStart + region->getSize() - 1;
			}

			// Assign region space to that memory space
			entry->memData = region->getBase();
		}

		// Allocate anonymous memory space for that address space
		if (entry->memData == nullptr && (entry->read.type == mapROMSpace ||
				entry->read.type == mapRAMSpace || entry->write.type == mapRAMSpace))
		{
			fmt::printf("%s(%s): %llX-%llX - allocating anonymous memory space\n", device.getDeviceName(), asInfo[space],
				entry->addrStart, entry->addrEnd);
			// entry->memData = manager.allocateMemory(this, entry->addrStart, entry->addrEnd,
			// 	entry->addrEnd - entry->addrStart + 1);
			entry->memData = manager.allocateMemory(device, space, "(anonymous)",
				config.convertAddressToByte(entry->addrEnd+1 - entry->addrStart),
				config.getDataWidth(), config.getEndianType());
		}
	}
}

void AddressSpace::populateEntry(const AddressEntry *entry, accessType rwType)
{
	mapHandler data = (rwType == accRead) ? entry->read : entry->write;

	switch(data.type)
	{
		case mapNone:
			return;
		case mapROMSpace:
			if (rwType == accWrite)
				return;
			// fallthrough
		case mapRAMSpace:
			setMemorySpace(entry->addrStart, entry->addrEnd, entry->addrMirror, entry->memData, rwType);
			break;
	}
}

void AddressSpace::populate(Console *cty)
{
	fmt::printf("%s: Populating for %s address space\n", device.getDeviceName(), asInfo[space]);

	assert(map != nullptr);

	for (AddressEntry *entry : map->list)
	{
		populateEntry(entry, accRead);
		populateEntry(entry, accWrite);
	}
}

void AddressSpace::allocate(Console *cty)
{
	fmt::printf("%s: Allocating memory space for %s address space\n", device.getDeviceName(), asInfo[space]);

	BlockList &blocks = manager.getBlockList();

	assert(map != nullptr);
	for (AddressEntry *entry : map->list)
	{
		if (entry->memData != nullptr && entry->addrStart == 0) {
			if (entry->addrStart == 0)
			{
				// Assign fast access for main memory
				// if starting address is zero.
				memData = entry->memData;
				memSize = entry->addrEnd + 1;
			}
//			blocks.push_back(new MemoryBlock(config, entry->addrStart, entry->addrEnd, entry->memData));
		}
	}

}

// **********************************************************************

cstag_t MemoryShare::compare(size_t bytes, int width, endian_t type) const
{
	if (width != bitWidth)
		return fmt::sprintf("Share '%s' found with unexpected width (expected %d, found %d)",
			name, width, bitWidth);
	if (bytes != size)
		return fmt::sprintf("Share '%s' found with unexpected size (expected %d, found %x)",
			name, bytes, size);
	if (type != this->type)
		return fmt::sprintf("Share '%s' found with unexpected endian type (expected %s, found %s)",
			name, type == LittleEndian ? "little" : "big",
			this->type == LittleEndian ? "little" : "big");
	return "";
}