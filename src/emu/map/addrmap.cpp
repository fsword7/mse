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
  addrStart(start), addrEnd(end),
  read8(dev), read16(dev), read32(dev), read64(dev),
  read8o(dev), read16o(dev), read32o(dev), read64o(dev),
  read8mo(dev), read16mo(dev), read32mo(dev), read64mo(dev),
  read8s(dev), read16s(dev), read32s(dev), read64s(dev),
  read8so(dev), read16so(dev), read32so(dev), read64so(dev),
  read8smo(dev), read16smo(dev), read32smo(dev), read64smo(dev),
  write8(dev), write16(dev), write32(dev), write64(dev),
  write8o(dev), write16o(dev), write32o(dev), write64o(dev),
  write8mo(dev), write16mo(dev), write32mo(dev), write64mo(dev),
  write8s(dev), write16s(dev), write32s(dev), write64s(dev),
  write8so(dev), write16so(dev), write32so(dev), write64so(dev),
  write8smo(dev), write16smo(dev), write32smo(dev), write64smo(dev)
{ }

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

AddressEntry &AddressEntry::r(read8d_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 8;
	read.name = func.getName();
	read8 = func;

	return *this;
}


AddressEntry &AddressEntry::r(read16d_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 16;
	read.name = func.getName();
	read16 = func;

	return *this;
}

AddressEntry &AddressEntry::r(read32d_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 32;
	read.name = func.getName();
	read32 = func;

	return *this;
}

AddressEntry &AddressEntry::r(read64d_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 64;
	read.name = func.getName();
	read64 = func;

	return *this;
}

AddressEntry &AddressEntry::r(read8do_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 8;
	read.name = func.getName();
	read8o = func;

	return *this;
}


AddressEntry &AddressEntry::r(read16do_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 16;
	read.name = func.getName();
	read16o = func;

	return *this;
}

AddressEntry &AddressEntry::r(read32do_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 32;
	read.name = func.getName();
	read32o = func;

	return *this;
}

AddressEntry &AddressEntry::r(read64do_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 64;
	read.name = func.getName();
	read64o = func;

	return *this;
}

AddressEntry &AddressEntry::r(read8dmo_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 8;
	read.name = func.getName();
	read8mo = func;

	return *this;
}


AddressEntry &AddressEntry::r(read16dmo_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 16;
	read.name = func.getName();
	read16mo = func;

	return *this;
}

AddressEntry &AddressEntry::r(read32dmo_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 32;
	read.name = func.getName();
	read32mo = func;

	return *this;
}

AddressEntry &AddressEntry::r(read64dmo_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 64;
	read.name = func.getName();
	read64mo = func;

	return *this;
}

// ********

AddressEntry &AddressEntry::r(read8ds_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 8;
	read.name = func.getName();
	read8s = func;

	return *this;
}


AddressEntry &AddressEntry::r(read16ds_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 16;
	read.name = func.getName();
	read16s = func;

	return *this;
}

AddressEntry &AddressEntry::r(read32ds_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 32;
	read.name = func.getName();
	read32s = func;

	return *this;
}

AddressEntry &AddressEntry::r(read64ds_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 64;
	read.name = func.getName();
	read64s = func;

	return *this;
}

AddressEntry &AddressEntry::r(read8dso_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 8;
	read.name = func.getName();
	read8so = func;

	return *this;
}


AddressEntry &AddressEntry::r(read16dso_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 16;
	read.name = func.getName();
	read16so = func;

	return *this;
}

AddressEntry &AddressEntry::r(read32dso_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 32;
	read.name = func.getName();
	read32so = func;

	return *this;
}

AddressEntry &AddressEntry::r(read64dso_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 64;
	read.name = func.getName();
	read64so = func;

	return *this;
}

AddressEntry &AddressEntry::r(read8dsmo_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 8;
	read.name = func.getName();
	read8smo = func;

	return *this;
}


AddressEntry &AddressEntry::r(read16dsmo_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 16;
	read.name = func.getName();
	read16smo = func;

	return *this;
}

AddressEntry &AddressEntry::r(read32dsmo_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 32;
	read.name = func.getName();
	read32smo = func;

	return *this;
}

AddressEntry &AddressEntry::r(read64dsmo_t func)
{
	assert(!func.isNull());

	read.type = mapDelegate;
	read.bits = 64;
	read.name = func.getName();
	read64smo = func;

	return *this;
}

// **********

AddressEntry &AddressEntry::w(write8d_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 8;
	write.name = func.getName();
	write8 = func;

	return *this;
}

AddressEntry &AddressEntry::w(write16d_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 16;
	write.name = func.getName();
	write16 = func;

	return *this;
}

AddressEntry &AddressEntry::w(write32d_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 32;
	write.name = func.getName();
	write32 = func;

	return *this;
}

AddressEntry &AddressEntry::w(write64d_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 64;
	write.name = func.getName();
	write64 = func;

	return *this;
}


AddressEntry &AddressEntry::w(write8do_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 8;
	write.name = func.getName();
	write8o = func;

	return *this;
}

AddressEntry &AddressEntry::w(write16do_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 16;
	write.name = func.getName();
	write16o = func;

	return *this;
}

AddressEntry &AddressEntry::w(write32do_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 32;
	write.name = func.getName();
	write32o = func;

	return *this;
}

AddressEntry &AddressEntry::w(write64do_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 64;
	write.name = func.getName();
	write64o = func;

	return *this;
}


AddressEntry &AddressEntry::w(write8dmo_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 8;
	write.name = func.getName();
	write8mo = func;

	return *this;
}

AddressEntry &AddressEntry::w(write16dmo_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 16;
	write.name = func.getName();
	write16mo = func;

	return *this;
}

AddressEntry &AddressEntry::w(write32dmo_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 32;
	write.name = func.getName();
	write32mo = func;

	return *this;
}

AddressEntry &AddressEntry::w(write64dmo_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 64;
	write.name = func.getName();
	write64mo = func;

	return *this;
}

// *********

AddressEntry &AddressEntry::w(write8ds_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 8;
	write.name = func.getName();
	write8s = func;

	return *this;
}

AddressEntry &AddressEntry::w(write16ds_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 16;
	write.name = func.getName();
	write16s = func;

	return *this;
}

AddressEntry &AddressEntry::w(write32ds_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 32;
	write.name = func.getName();
	write32s = func;

	return *this;
}

AddressEntry &AddressEntry::w(write64ds_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 64;
	write.name = func.getName();
	write64s = func;

	return *this;
}


AddressEntry &AddressEntry::w(write8dso_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 8;
	write.name = func.getName();
	write8so = func;

	return *this;
}

AddressEntry &AddressEntry::w(write16dso_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 16;
	write.name = func.getName();
	write16so = func;

	return *this;
}

AddressEntry &AddressEntry::w(write32dso_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 32;
	write.name = func.getName();
	write32so = func;

	return *this;
}

AddressEntry &AddressEntry::w(write64dso_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 64;
	write.name = func.getName();
	write64so = func;

	return *this;
}


AddressEntry &AddressEntry::w(write8dsmo_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 8;
	write.name = func.getName();
	write8smo = func;

	return *this;
}

AddressEntry &AddressEntry::w(write16dsmo_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 16;
	write.name = func.getName();
	write16smo = func;

	return *this;
}

AddressEntry &AddressEntry::w(write32dsmo_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 32;
	write.name = func.getName();
	write32smo = func;

	return *this;
}

AddressEntry &AddressEntry::w(write64dsmo_t func)
{
	assert(!func.isNull());

	write.type = mapDelegate;
	write.bits = 64;
	write.name = func.getName();
	write64smo = func;

	return *this;
}


// ******** AddressList ********

AddressList::AddressList(device_t &dev, int space)
: device(dev), addrSpace(space)
{
	diExternalBus *sbus;
	const AddressConfig *config;

	// fmt::printf("%s: Set address list at space %d\n", dev.getDeviceName(), space);

	list.clear();

	dev.hasInterface(sbus);
	assert(sbus != nullptr);

	config = sbus->getAddressConfig(space);
	assert(config != nullptr);

	Constructor map = sbus->getAddressMap(space);
	if (!map.isNull()) {
//		fmt::printf("%s: Trying call bus initialization delegate\n", dev.getDeviceName());
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
