/*
 * addrmap.h - Address Mapping package
 *
 *  Created on: Jul 18, 2020
 *      Author: Tim Stark
 */

#pragma once

// #define FUNC(fnc) &fnc, #fnc

namespace aspace
{
	class AddressList;
	class AddressSpace;

	enum mapType
	{
		mapNone = 0,
		mapAccess,		// Accessible
		mapROMSpace,	// ROM space
		mapRAMSpace,	// RAM space
		mapPort,		// I/O type access
		mapBank,        // Bank space access
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
			return &static_cast<T &>(obj);
		}

		// RAM/ROM access list
		AddressEntry &ram()   { read.type = mapRAMSpace; write.type = mapRAMSpace; return *this; }
		AddressEntry &rom()   { read.type = mapROMSpace; write.type = mapNop;      return *this; }
		AddressEntry &ronly() { read.type = mapRAMSpace;  return *this; }
		AddressEntry &wonly() { write.type = mapRAMSpace; return *this; }

		AddressEntry &portr(ctag_t *name)    { read.type = mapPort; read.name = name; return *this; }
		AddressEntry &portw(ctag_t *name)    { write.type = mapPort; write.name = name; return *this; }
		AddressEntry &portrw(ctag_t *name)   { portr(name); portw(name); return *this; }
	
		AddressEntry &bankr(ctag_t *name)    { read.type = mapBank; read.name = name; return *this; }
		AddressEntry &bankw(ctag_t *name)    { write.type = mapBank; write.name = name; return *this; }
		AddressEntry &bankrw(ctag_t *name)   { bankr(name); bankw(name); return *this; }

		// User-defined memory configuration management
		AddressEntry &expandable()       { expFlag = true; return *this; }
		AddressEntry &unexpandable()     { expFlag = false; return *this; }
		AddressEntry &size(offs_t size)  { memSize = size; return *this; }

		AddressEntry &mirror(offs_t bits) { addrMirror = bits; return *this; }

		AddressEntry &region(ctag_t *name, offs_t off = 0);
		AddressEntry &share(ctag_t *name) { shareName = name; return *this; }

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

		template <typename T, typename U, typename Ret, typename ... Args>
		AddressEntry &r(T &&obj, Ret (U::*read)(Args...), ctag_t *readName)
		{
			return r(makeDelegate(*make_pointer<U>(obj), read, readName));
		}

		template <typename T, typename U, typename Ret, typename ... Args>
		AddressEntry &w(T &&obj, Ret (U::*write)(Args...), ctag_t *writeName)
		{
			return w(makeDelegate(*make_pointer<U>(obj), write, writeName));
		}

		template <typename T, typename U, typename rRet, typename... rArgs, typename V, typename wRet, typename... wArgs>
		AddressEntry &rw(T &&obj, rRet (U::*read)(rArgs...), ctag_t *readName, wRet (V::*write)(wArgs...), ctag_t *writeName)
		{
			return r(makeDelegate(*make_pointer<U>(obj), read, readName))
				  .w(makeDelegate(*make_pointer<V>(obj), write, writeName));
		}


		template <typename T>
		AddressEntry &dr8(T &&read, ctag_t *name)
		{
			return r(makeDelegate8(device, std::forward<T>(read), name));
		}

		template <typename T>
		AddressEntry &dr16(T &&read, ctag_t *name)
		{
			return r(makeDelegate16(device, std::forward<T>(read), name));
		}

		template <typename T>
		AddressEntry &dr32(T &&read, ctag_t *name)
		{
			return r(makeDelegate32(device, std::forward<T>(read), name));
		}

		template <typename T>
		AddressEntry &dr64(T &&read, ctag_t *name)
		{
			return r(makeDelegate64(device, std::forward<T>(read), name));
		}


		template <typename T>
		AddressEntry &dw8(T &&write, ctag_t *name)
		{
			return w(makeDelegate8(device, std::forward<T>(read), name));
		}

		template <typename T>
		AddressEntry &dw16(T &&write, ctag_t *name)
		{
			return w(makeDelegate16(device, std::forward<T>(read), name));
		}

		template <typename T>
		AddressEntry &dw32(T &&write, ctag_t *name)
		{
			return w(makeDelegate32(device, std::forward<T>(read), name));
		}

		template <typename T>
		AddressEntry &dw64(T &&write, ctag_t *name)
		{
			return w(makeDelegate64(device, std::forward<T>(read), name));
		}


		template <typename T, typename U>
		AddressEntry &drw8(T &&read, ctag_t *readName, U &&write, ctag_t *writeName)
		{
			return r(makeDelegate8(device, std::forward<T>(read), readName))
			      .w(makeDelegate8(device, std::forward<U>(write), writeName));
		}

		template <typename T, typename U>
		AddressEntry &drw16(T &&read, ctag_t *readName, U &&write, ctag_t *writeName)
		{
			return r(makeDelegate16(device, std::forward<T>(read), readName))
				  .w(makeDelegate16(device, std::forward<U>(write), writeName));
		}

		template <typename T, typename U>
		AddressEntry &drw32(T &&read, ctag_t *readName, U &&write, ctag_t *writeName)
		{
			return r(makeDelegate32(device, std::forward<T>(read), readName))
				  .w(makeDelegate32(device, std::forward<U>(write), writeName));
		}

		template <typename T, typename U>
		AddressEntry &drw64(T &&read, ctag_t *readName, U &&write, ctag_t *writeName)
		{
			return r(makeDelegate64(device, std::forward<T>(read), readName))
				  .w(makeDelegate64(device, std::forward<U>(write), writeName));
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

		// Memory configuration
		uint8_t  *memData = nullptr;
		uint64_t  memSize = 0;
		uint64_t  maxSize = 0;
		bool      expFlag = false;

		// Memory region parameters
		ctag_t *regionName = nullptr;
		offs_t  regionOffset = 0;

		// Memory share parameters
		ctag_t *shareName = nullptr;

		// read/write access handler
		mapHandler read, write;

		// Device access handlers
		read8d_t    read8;
		read16d_t   read16;
		read32d_t   read32;
		read64d_t   read64;

		read8do_t    read8o;
		read16do_t   read16o;
		read32do_t   read32o;
		read64do_t   read64o;

		read8dmo_t    read8mo;
		read16dmo_t   read16mo;
		read32dmo_t   read32mo;
		read64dmo_t   read64mo;

		read8ds_t    read8s;
		read16ds_t   read16s;
		read32ds_t   read32s;
		read64ds_t   read64s;

		read8dso_t    read8so;
		read16dso_t   read16so;
		read32dso_t   read32so;
		read64dso_t   read64so;

		read8dsmo_t    read8smo;
		read16dsmo_t   read16smo;
		read32dsmo_t   read32smo;
		read64dsmo_t   read64smo;


		write8d_t   write8;
		write16d_t  write16;
		write32d_t  write32;
		write64d_t  write64;
		
		write8do_t   write8o;
		write16do_t  write16o;
		write32do_t  write32o;
		write64do_t  write64o;

		write8dmo_t   write8mo;
		write16dmo_t  write16mo;
		write32dmo_t  write32mo;
		write64dmo_t  write64mo;

		write8ds_t   write8s;
		write16ds_t  write16s;
		write32ds_t  write32s;
		write64ds_t  write64s;
		
		write8dso_t   write8so;
		write16dso_t  write16so;
		write32dso_t  write32so;
		write64dso_t  write64so;

		write8dsmo_t   write8smo;
		write16dsmo_t  write16smo;
		write32dsmo_t  write32smo;
		write64dsmo_t  write64smo;

	};

	class AddressList
	{
		friend class AddressEntry;
		friend class AddressSpace;

	public:
		using offs_t = uint64_t;

		AddressList(device_t &dev, int space);
		~AddressList();

		inline void setGlobalAddressMask(offs_t mask)	{ gaddrMask = mask; }
		inline void setUnmap(uint8_t val)				{ unmapValue = val; }
		inline void setUnmapHigh()						{ unmapValue = ~0; }
		inline void setUnmapLow()						{ unmapValue = 0; }

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
