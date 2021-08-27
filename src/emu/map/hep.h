/*
 * hep.h - Handler entry - passthrough access
 *
 *  Created on: Aug 26, 2021
 *      Author: Tim Stark
 */

#pragma once

namespace aspace
{

	template <int dWidth, int aShift>
	class HandlerReadPassthrough : public HandlerReadAddress<dWidth, aShift>
	{
	public:
		using uintx_t = typename HandlerSize<dWidth>::uintx_t;
		using inh = HandlerReadAddress<dWidth, aShift>;

		HandlerReadPassthrough(AddressSpace *space)
		: HandlerReadAddress<dWidth, aShift>(space, HandlerEntry::hePassthrough)
		{ }

		~HandlerReadPassthrough() = default;

		string getName() const override { return "passthrough"; }
		// void setBase(uintx_t *base) override { baseData = base; }

		uintx_t read(offs_t offset, cpuDevice *cpu) const override
		{
			// assert(baseData != nullptr);
			// return baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
			return 0;
		}

		uintx_t read(offs_t offset, uintx_t mask, cpuDevice *cpu) const override
		{
			// assert(baseData != nullptr);
			// return baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
			return 0;
		}

		void *getAccess(offs_t offset) const override
		{
			// assert(baseData != nullptr);
			// return &baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
			return nullptr;
		}

	private:
	};

	template <int dWidth, int aShift>
	class HandlerWritePassthrough : public HandlerWriteAddress<dWidth, aShift>
	{
	public:
		using uintx_t = typename HandlerSize<dWidth>::uintx_t;
		using inh = HandlerWriteAddress<dWidth, aShift>;

		HandlerWritePassthrough(AddressSpace *space)
		: HandlerWriteAddress<dWidth, aShift>(space, HandlerEntry::hePassthrough)
		{ }

		~HandlerWritePassthrough() = default;

		string getName() const override { return "passthrough"; }
		// void setBase(uintx_t *base) override { baseData = base; }

		void write(offs_t offset, uintx_t data, cpuDevice *cpu) const override
		{
			// assert(baseData != nullptr);
			// baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)] = data;
		}

		void write(offs_t offset, uintx_t data, uintx_t mask, cpuDevice *cpu) const override
		{
			// assert(baseData != nullptr);
			// offs_t off = ((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift);
			// baseData[off] = (baseData[off] & ~mask) | (data & mask);
		}

		void *getAccess(offs_t offset) const override
		{
			// assert(baseData != nullptr);
			// return &baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
			return nullptr;
		}

	private:
	};

}
