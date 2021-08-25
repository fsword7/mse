/*
 * he.h - handler entry package
 *
 *  Created on: Jan 10, 2021
 *      Author: Tim Stark
 */

#pragma once

// Address width definition types
//
// 0 -  8-bit address width
// 1 - 16-bit address width
// 2 - 32-bit address width
// 3 - 64-bit address width

// High bit address for paging
#define HB_LEVEL3 48	// 48-63 address width
#define HB_LEVEL2 32	// 32-47 address width
#define HB_LEVEL1 16	// 16-31 address width
#define HB_LEVEL0 0		// 0-15 address width

namespace aspace
{

	// Data width - types
	template <int dWidth> struct HandlerSize { };
	template <> struct HandlerSize<0> { using uintx_t = uint8_t; };
	template <> struct HandlerSize<1> { using uintx_t = uint16_t; };
	template <> struct HandlerSize<2> { using uintx_t = uint32_t; };
	template <> struct HandlerSize<3> { using uintx_t = uint64_t; };

	constexpr int determineDispatchLevel(int aWidth) // address width access
	{
		return	(aWidth > HB_LEVEL3) ? 3 :
				(aWidth > HB_LEVEL2) ? 2 :
				(aWidth > HB_LEVEL1) ? 1 :
				0;
	}

	constexpr int determineDispatchLowBits(int aWidth, int dWidth, int aShift)
	{
		return (aWidth > HB_LEVEL3) ? HB_LEVEL3 :
			   (aWidth > HB_LEVEL2) ? HB_LEVEL2 :
			   (aWidth > HB_LEVEL1) ? HB_LEVEL1 :
			   (dWidth - aShift);
	}

	class HandlerEntry
	{
	public:
		HandlerEntry(AddressSpace *space, uint64_t flags)
		: space(space), flags(flags), refCount(1)
		{ }

		virtual ~HandlerEntry() {}


		virtual string getName() = 0;

		inline void ref(int count = 1)
		{
			refCount += count;
		}

		inline void unref(int count = 1)
		{
			refCount -= count;
			if (refCount == 0)
				delete this;
		}

	protected:
		AddressSpace *space;
		uint64_t      flags;
		mutable int   refCount = 0;

		struct range
		{
			offs_t start;
			offs_t end;
		};
	};

	template <int dWidth, int aShift>
	class HandlerRead : public HandlerEntry
	{
	public:
		using uintx_t = typename HandlerSize<dWidth>::uintx_t;

		static constexpr offs_t nativeMask = (dWidth - aShift) >= 0 ? makeBitmask<offs_t>(dWidth - aShift) : 0;

		HandlerRead(AddressSpace *space, uint64_t flags)
		: HandlerEntry(space, flags)
		{ }

		virtual ~HandlerRead();

		virtual uintx_t read(offs_t off) = 0;
		virtual uintx_t read(offs_t off, uintx_t mask) = 0;
		virtual void *getAccess(offs_t off) const { return nullptr; }

		// inline void populate(offs_t sAddr, offs_t eAddr, offs_t mAddr)
		// {
		// 	// sAddr &= ~nativeMask;
		// 	// eAddr |= nativeMask;
		// 	if (mAddr != 0)
		// 		populateMirror(sAddr, eAddr, sAddr, eAddr, mAddr);
		// 	else
		// 		populateNoMirror(sAddr, eAddr, sAddr, eAddr);
		// }

		// virtual void populateMirror(offs_t sAddr, offs_t eAddr, offs_t sOrgin, offs_t eOrgin, offs_t mirror) = 0;
		// virtual void populateNoMirror(offs_t sAddr, offs_t eAddr, offs_t sOrgin, offs_t eOrgin) = 0;
	};

	template <int dWidth, int aShift>
	class HandlerWrite : public HandlerEntry
	{
	public:
		using uintx_t = typename HandlerSize<dWidth>::uintx_t;

		static constexpr offs_t nativeMask = (dWidth - aShift) >= 0 ? makeBitmask<offs_t>(dWidth - aShift) : 0;

		HandlerWrite(AddressSpace *space, uint64_t flags)
		: HandlerEntry(space, flags)
		{ }

		virtual ~HandlerWrite();

		virtual void write(offs_t off, uintx_t data) = 0;
		virtual void write(offs_t off, uintx_t data, uintx_t mask) = 0;
		virtual void *getAccess(offs_t off) const { return nullptr; }

		// inline void populate(offs_t sAddr, offs_t eAddr, offs_t mAddr)
		// {
		// 	// sAddr &= ~nativeMask;
		// 	// eAddr |= nativeMask;
		// 	if (mAddr != 0)
		// 		populateMirror(sAddr, eAddr, sAddr, eAddr, mAddr);
		// 	else
		// 		populateNoMirror(sAddr, eAddr, sAddr, eAddr);
		// }

		// virtual void populateMirror(offs_t sAddr, offs_t eAddr, offs_t sOrgin, offs_t eOrgin, offs_t mirror) = 0;
		// virtual void populateNoMirror(offs_t sAddr, offs_t eAddr, offs_t sOrgin, offs_t eOrgin) = 0;
	};
}
