/*
 * hedw.h - Write Handler Entry package - Dispatch
 *
 *  Created on: Apr 5, 2021
 *      Author: Tim Stark
 */

#pragma once

namespace aspace
{

	template <int highBits, int dWidth, int aShift>
	class HandlerWriteDispatch : public HandlerWrite<dWidth, aShift>
	{
	public:

		using uintx_t = typename HandlerSize<dWidth>::uintx_t;
		HandlerWriteDispatch(AddressSpace *space, const HandlerEntry::range &init,
			HandlerWrite<dWidth, aShift> *handler = nullptr)
		: HandlerWrite<dWidth, aShift>(space, 0)
		{
			// Initialize dispatch table
			assert(handler != nullptr);
			handler->ref(count);
			for (int idx = 0; idx < count; idx++)
			{
				dispatch[idx] = handler;
				ranges[idx] = init;
			}
		}

		~HandlerWriteDispatch()
		{
			// Clear all dispatch table
			for (int idx = 0; idx < count; idx++)
				dispatch[idx]->unref();
		}

		string getName() const override { return "dispatch"; }

		void write(offs_t offset, uintx_t data) override
		{
			offs_t off = (offset >> lowBits) & bitMask;
			assert(dispatch[off] != nullptr);
			dispatch[off]->write(offset, data);
		}

		void write(offs_t offset, uintx_t data, uintx_t mask) override
		{
			offs_t off = (offset >> lowBits) & bitMask;
			assert(dispatch[off] != nullptr);
			dispatch[off]->write(offset, data, mask);
		}

	protected:
		static constexpr uint32_t lowBits  = determineDispatchLowBits(highBits, dWidth, aShift);
		static constexpr uint32_t bitCount = highBits > lowBits ? highBits - lowBits : 0;
		static constexpr uint32_t count    = 1u << bitCount;

		static constexpr offs_t   bitMask  = (1u << bitCount) - 1;

		HandlerWrite<dWidth, aShift> *dispatch[count];
		HandlerEntry::range ranges[count];
	};

}
