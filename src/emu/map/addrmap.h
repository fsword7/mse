/*
 * addrmap.h - Address Mapping package
 *
 *  Created on: Jul 18, 2020
 *      Author: Tim Stark
 */

#pragma once

class mapAddress;

enum mapType
{
	mapNone = 0,
	mapAccess,	// Accessible
	mapPort,	// I/O type access
	mapNop,		// Non-operation - do nothing
	mapVoid		// Non-existent - exception trap
};

struct mapHandler
{
	mapType		type  = mapNone;
	uint16_t	bits  = 0; // width bits, 0 = default
	const char	*name = nullptr;
};

class mapAddressEntry
{
public:
	using offs_t = uint64_t;

	mapAddressEntry(device_t &dev, mapAddress &map, offs_t start, offs_t end);

	// RAM/ROM access list
	mapAddressEntry &ram()   { read.type = mapAccess; write.type = mapAccess; return *this; }
	mapAddressEntry &rom()   { read.type = mapAccess; write.type = mapNop;    return *this; }
	mapAddressEntry &ronly() { read.type = mapAccess;  return *this; }
	mapAddressEntry &wonly() { write.type = mapAccess; return *this; }

	mapAddressEntry &mirror(offs_t bits) { addrMirror = bits; return *this; }

	mapAddressEntry &mask(offs_t mask);

public:
	// Address entry information
	mapAddressEntry *mapNext = nullptr;
	mapAddress      &map;
	device_t        &device;

	// Address parameters
	offs_t addrStart;		// Start address
	offs_t addrEnd;			// End address
	offs_t addrMask = 0;	// Mask address bits
	offs_t addrMirror = 0;	// Mirror address bits

	// read/write access handler
	mapHandler read, write;
};

class mapAddress
{
	friend class mapAddressEntry;

public:
	using offs_t = uint64_t;

	mapAddress(device_t &dev, int space);
	~mapAddress();

	inline void setGlobalAddressMask(offs_t mask) { addrMask = mask; }

	mapAddressEntry &operator ()(offs_t start, offs_t end);

private:
	device_t &device;
	int addrSpace;

	offs_t addrMask = 0; // Global physical address mask

	vector<mapAddressEntry *> list;
};
