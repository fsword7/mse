/*
 * addrmap.h - Address Mapping package
 *
 *  Created on: Jul 18, 2020
 *      Author: Tim Stark
 */

#pragma once

namespace map
{
	class AddressList;

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
		mapType   type = mapNone;
		uint16_t  bits = 0; // width bits, 0 = default
		ctag_t   *name = nullptr;
	};

	class AddressEntry
	{
	public:
		using offs_t = uint64_t;

		AddressEntry(device_t &dev, AddressList &map, offs_t start, offs_t end);

		// RAM/ROM access list
		AddressEntry &ram()   { read.type = mapAccess; write.type = mapAccess; return *this; }
		AddressEntry &rom()   { read.type = mapAccess; write.type = mapNop;    return *this; }
		AddressEntry &ronly() { read.type = mapAccess;  return *this; }
		AddressEntry &wonly() { write.type = mapAccess; return *this; }

		AddressEntry &mirror(offs_t bits) { addrMirror = bits; return *this; }

		AddressEntry &mask(offs_t mask);

	public:
		// Address entry information
		AddressEntry  *mapNext = nullptr;
		AddressList   &map;
		device_t      &device;

		// Address parameters
		offs_t addrStart  = 0;   // Start address
		offs_t addrEnd    = 0;   // End address
		offs_t addrMask   = 0;   // Mask address bits
		offs_t addrMirror = 0;   // Mirror address bits

		// read/write access handler
		mapHandler read, write;
	};

	class AddressList
	{
		friend class AddressEntry;

	public:
		using offs_t = uint64_t;

		AddressList(device_t &dev, int space);
		~AddressList();

		inline void setGlobalAddressMask(offs_t mask) { gaddrMask = mask; }

		inline int size() const { return list.size(); }

		AddressEntry &operator ()(offs_t start, offs_t end);

	private:
		device_t &device;
		int addrSpace;

		offs_t   gaddrMask  = 0;
		uint64_t unmapValue = 0;

		vector<AddressEntry *> list;
	};
}
