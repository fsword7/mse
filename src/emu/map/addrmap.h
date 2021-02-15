/*
 * addrmap.h - Address Mapping package
 *
 *  Created on: Jul 18, 2020
 *      Author: Tim Stark
 */

#pragma once

namespace aspace
{
	class AddressList;
	class AddressSpace;

	enum mapType
	{
		mapNone = 0,
		mapAccess,		// Accessible
		mapPort,		// I/O type access
		mapDelegate,	// Device delegate (callbacks)
		mapNop,			// Non-operation - do nothing
		mapVoid			// Non-existent - exception trap
	};

	struct mapHandler
	{
		mapType   type = mapNone;
		uint16_t  bits = 0; // width bits, 0 = default
		ctag_t   *name = nullptr;
	};

	class AddressEntry
	{
		friend class AddressSpace;

	public:
		using offs_t = uint64_t;

		AddressEntry(device_t &dev, AddressList &map, offs_t start, offs_t end);

		template <typename T, typename U>
		static std::enable_if_t<std::is_convertible<std::add_pointer_t<U>, std::add_pointer_t<T>>::value, T *>
			make_pointer(U &obj)
		{
			return &dynamic_cast<T &>(obj);
		}

		template <typename T, typename U>
		static std::enable_if_t<!std::is_convertible<std::add_pointer_t<U>, std::add_pointer_t<T>>::value, T *>
			make_pointer(U &obj)
		{
			return &dynamic_cast<T &>(obj);
		}

		// RAM/ROM access list
		AddressEntry &ram()   { read.type = mapAccess; write.type = mapAccess; return *this; }
		AddressEntry &rom()   { read.type = mapAccess; write.type = mapNop;    return *this; }
		AddressEntry &ronly() { read.type = mapAccess;  return *this; }
		AddressEntry &wonly() { write.type = mapAccess; return *this; }

		AddressEntry &mirror(offs_t bits) { addrMirror = bits; return *this; }

		AddressEntry &region(ctag_t *name, offs_t off = 0);

		AddressEntry &mask(offs_t mask);

		AddressEntry &r(read8d_t func);
		AddressEntry &r(read16d_t func);
		AddressEntry &r(read32d_t func);
		AddressEntry &r(read64d_t func);

		AddressEntry &w(write8d_t func);
		AddressEntry &w(write16d_t func);
		AddressEntry &w(write32d_t func);
		AddressEntry &w(write64d_t func);

		// Implicit delegate calls
		template <typename T, typename rRet, typename ... rArgs>
		AddressEntry &r(rRet (T::*read)(rArgs...), ctag_t *readName)
		{
			return r(makeDelegate(read, make_pointer<T>(device), readName, device.getDeviceName()));
		}

		template <typename U, typename wRet, typename ... wArgs>
		AddressEntry &w(wRet (U::*write)(wArgs...), ctag_t *writeName)
		{
			return w(makeDelegate(write, make_pointer<U>(device), writeName, device.getDeviceName()));
		}

		template <typename T, typename rRet, typename ... rArgs, typename U, typename wRet, typename ... wArgs>
		AddressEntry &rw(rRet (T::*read)(rArgs...), ctag_t *readName,
			             wRet (U::*write)(wArgs...), ctag_t *writeName)
		{
			r(makeDelegate(read, make_pointer<T>(device), readName, device.getDeviceName()));
			w(makeDelegate(write, make_pointer<T>(device), writeName, device.getDeviceName()));
			return *this;
		}

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

		uint8_t *memData = nullptr;

		// Memory region parameters
		ctag_t *regionName = nullptr;
		offs_t  regionOffset = 0;

		// read/write access handler
		mapHandler read, write;

		// Device access handlers
		read8d_t    read8;
		read16d_t   read16;
		read32d_t   read32;
		read64d_t   read64;

		write8d_t   write8;
		write16d_t  write16;
		write32d_t  write32;
		write64d_t  write64;
	};

	class AddressList
	{
		friend class AddressEntry;
		friend class AddressSpace;

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
