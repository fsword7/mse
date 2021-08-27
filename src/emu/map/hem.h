/*
 * hem.h - Handler entry - memory access
 *
 *  Created on: Apr 3, 2021
 *      Author: Tim Stark
 */

#pragma once

namespace aspace
{

	template <int dWidth, int aShift>
	class HandlerReadMemory : public HandlerReadAddress<dWidth, aShift>
	{
	public:
		using uintx_t = typename HandlerSize<dWidth>::uintx_t;
		using inh = HandlerReadAddress<dWidth, aShift>;

		HandlerReadMemory(AddressSpace *space, void *base)
		: HandlerReadAddress<dWidth, aShift>(space, 0),
		  baseData(reinterpret_cast<uintx_t *>(base))
		{ }

		~HandlerReadMemory() = default;

		string getName() const override { return "memory"; }
		// void setBase(uintx_t *base) override { baseData = base; }

		uintx_t read(offs_t offset, cpuDevice *cpu) const override
		{
			assert(baseData != nullptr);
			return baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
		}

		uintx_t read(offs_t offset, uintx_t mask, cpuDevice *cpu) const override
		{
			assert(baseData != nullptr);
			return baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
		}

		void *getAccess(offs_t offset) const override
		{
			assert(baseData != nullptr);
			return &baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
		}

	private:
		mutable uintx_t *baseData = nullptr;
	};

	template <int dWidth, int aShift>
	class HandlerWriteMemory : public HandlerWriteAddress<dWidth, aShift>
	{
	public:
		using uintx_t = typename HandlerSize<dWidth>::uintx_t;
		using inh = HandlerWriteAddress<dWidth, aShift>;

		HandlerWriteMemory(AddressSpace *space, void *base)
		: HandlerWriteAddress<dWidth, aShift>(space, 0),
		  baseData(reinterpret_cast<uintx_t *>(base))
		{ }

		~HandlerWriteMemory() = default;

		string getName() const override { return "memory"; }
		// void setBase(uintx_t *base) override { baseData = base; }

		void write(offs_t offset, uintx_t data, cpuDevice *cpu) const override
		{
			assert(baseData != nullptr);
			baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)] = data;
		}

		void write(offs_t offset, uintx_t data, uintx_t mask, cpuDevice *cpu) const override
		{
			assert(baseData != nullptr);
			offs_t off = ((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift);
			baseData[off] = (baseData[off] & ~mask) | (data & mask);
		}

		void *getAccess(offs_t offset) const override
		{
			assert(baseData != nullptr);
			return &baseData[((offset - inh::baseAddress) & inh::maskAddress) >> (dWidth + aShift)];
		}

	private:
		mutable uintx_t *baseData = nullptr;
	};

}
