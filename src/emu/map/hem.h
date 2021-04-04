/*
 * hem.h - Handler entry - memory access
 *
 *  Created on: Apr 3, 2021
 *      Author: Tim Stark
 */

#pragma once

namespace aspace
{

	template <int dWidth, int aShift, int Endian>
	class HandlerReadMemory : public HandlerReadAddress<dWidth, aShift, Endian>
	{
	public:
		using uintx_t = typename HandlerSize<dWidth>::uintx_t;
		using inh = HandlerReadAddress<dWidth, aShift, Endian>;

		HandlerReadMemory(AddressSpace *space)
		: HandlerReadAddress<dWidth, aShift, Endian>(space, 0)
		{ }

		~HandlerReadMemory() = default;

		inline ctag_t *getName() const { return "memory"; }
		inline void setBase(uintx_t *base) { baseData = base; }

		uintx_t read(offs_t offset, uintx_t mask)
		{
			assert(baseData != nullptr);
			return baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
		}

		uintx_t *access(offs_t offset)
		{
			assert(baseData != nullptr);
			return &baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
		}

	private:
		uintx_t *baseData = nullptr;
	};

	template <int dWidth, int aShift, int Endian>
	class HandlerWriteMemory : public HandlerWriteAddress<dWidth, aShift, Endian>
	{
	public:
		using uintx_t = typename HandlerSize<dWidth>::uintx_t;
		using inh = HandlerWriteAddress<dWidth, aShift, Endian>;

		HandlerWriteMemory(AddressSpace *space)
		: HandlerWriteAddress<dWidth, aShift, Endian>(space, 0)
		{ }

		~HandlerWriteMemory() = default;

		inline ctag_t *getName() const { return "memory"; }
		inline void setBase(uintx_t *base) { baseData = base; }

		void write(offs_t offset, uintx_t data)
		{
			assert(baseData != nullptr);
			baseData[((offset - inh::baseAddress) & inh::maskAddrss) >> (dWidth + aShift)] = data;
		}

		void write(offs_t offset, uintx_t data, uintx_t mask)
		{
			assert(baseData != nullptr);
			offs_t off = ((offset - inh::baseAddress) & inh::maskAddrss) >> (dWidth + aShift);
			baseData[off] = (baseData[off] & ~mask) | (data & mask);
		}

		uintx_t *access(offs_t offset)
		{
			assert(baseData != nullptr);
			return &baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
		}

	private:
		uintx_t *baseData = nullptr;
	};

}
