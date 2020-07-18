/*
 * addrmap.cpp - Address mapping package
 *
 *  Created on: Jul 18, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/device.h"
#include "emu/map/addrmap.h"

// ******** mapAddressEntry ********

mapAddressEntry::mapAddressEntry(device_t &dev, mapAddress &map, offs_t start, offs_t end)
: device(dev), map(map),
  addrStart(start), addrEnd(end)
{

}

mapAddressEntry &mapAddressEntry::mask(offs_t mask)
{
	addrMask = mask;
	// Apply with global address mask
	if (map.addrMask != 0)
		addrMask &= ~map.addrMask;
	return *this;
}

// ******** mapAddress ********

mapAddress::mapAddress(device_t &dev, int space)
: device(dev), addrSpace(space)
{
	list.clear();
}

mapAddress::~mapAddress()
{
	// Delete all address mapping entries
	for (int idx = 0; idx < list.size(); idx++)
		delete list[idx];
	list.clear();
}

mapAddressEntry &mapAddress::operator ()(offs_t start, offs_t end)
{
	mapAddressEntry *entry;

	entry = new mapAddressEntry(device, *this, start, end);
	list.push_back(entry);

	return *entry;
}
