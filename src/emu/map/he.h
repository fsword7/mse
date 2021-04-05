/*
 * he.h - handler entry package
 *
 *  Created on: Jan 10, 2021
 *      Author: Tim Stark
 */

#pragma once

#define HB_LEVEL3 48
#define HB_LEVEL2 32
#define HB_LEVEL1 16

namespace aspace
{

	// Data width - types
	template <int dWidth> struct HandlerSize { };
	template <> struct HandlerSize<0> { using uintx_t = uint8_t; };
	template <> struct HandlerSize<1> { using uintx_t = uint16_t; };
	template <> struct HandlerSize<2> { using uintx_t = uint32_t; };
	template <> struct HandlerSize<3> { using uintx_t = uint64_t; };

	constexpr int determineDispatchLowBits(int highBits, int dWidth, int aShift)
	{
		return (highBits > HB_LEVEL3) ? HB_LEVEL3 :
			   (highBits > HB_LEVEL2) ? HB_LEVEL2 :
			   (highBits > HB_LEVEL1) ? HB_LEVEL1 :
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

	template <int dWidth, int aShift, int endian>
	class HandlerRead : public HandlerEntry
	{
	public:
		using uintx_t = typename HandlerSize<dWidth>::uintx_t;


		HandlerRead(AddressSpace *space, uint64_t flags)
		: HandlerEntry(space, flags)
		{ }

		virtual ~HandlerRead();

		virtual uintx_t read(offs_t off) = 0;
		virtual uintx_t read(offs_t off, uintx_t mask) = 0;
		virtual void *getAccess(offs_t off) const { return nullptr; }
	};

	template <int dWidth, int aShift, int endian>
	class HandlerWrite : public HandlerEntry
	{
	public:
		using uintx_t = typename HandlerSize<dWidth>::uintx_t;


		HandlerWrite(AddressSpace *space, uint64_t flags)
		: HandlerEntry(space, flags)
		{ }

		virtual ~HandlerWrite();

		virtual void write(offs_t off, uintx_t data) = 0;
		virtual void write(offs_t off, uintx_t data, uintx_t mask) = 0;
		virtual void *getAccess(offs_t off) const { return nullptr; }
	};
}
