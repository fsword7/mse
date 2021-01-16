/*
 * addrmap.cpp - Address mapping package
 *
 *  Created on: Jul 18, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/device.h"
#include "emu/dibus.h"
#include "emu/map/addrmap.h"

using namespace aspace;

// ******** AddressEntry ********

AddressEntry::AddressEntry(device_t &dev, AddressList &map, offs_t start, offs_t end)
: device(dev), map(map),
  addrStart(start), addrEnd(end)
{

}

AddressEntry &AddressEntry::region(ctag_t *name, offs_t off)
{
	// Assign region space to that entry
	regionName   = name;
	regionOffset = off;

	return *this;
}

AddressEntry &AddressEntry::mask(offs_t mask)
{
	addrMask = mask;
	// Apply with global address mask
	if (map.gaddrMask != 0)
		addrMask &= ~map.gaddrMask;
	return *this;
}

// ******** AddressList ********

AddressList::AddressList(device_t &dev, int space)
: device(dev), addrSpace(space)
{
	diExternalBus *sbus;
	const AddressConfig *config;

	list.clear();

	dev.hasInterface(sbus);
	assert(sbus != nullptr);

	config = sbus->getAddressConfig(space);
	assert(config != nullptr);

	Constructor map = sbus->getAddressMap(space);
	if (!map.isNull()) {
		fmt::printf("%s: Trying call bus initialization delegate\n", dev.getDeviceName());
		map(*this);
	}
}

AddressList::~AddressList()
{
	// Delete all address mapping entries
	for (int idx = 0; idx < list.size(); idx++)
		delete list[idx];
	list.clear();
}

AddressEntry &AddressList::operator ()(offs_t start, offs_t end)
{
	AddressEntry *entry;

	entry = new AddressEntry(device, *this, start, end);
	list.push_back(entry);

	return *entry;
}
