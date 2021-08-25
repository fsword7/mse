/*
 * hedr.h - Read Handler Entry package - Dispatch
 *
 *  Created on: Apr 5, 2021
 *      Author: Tim Stark
 */

#pragma once

namespace aspace
{
	template <int highBits, int dWidth, int aShift>
	class HandlerReadDispatch : public HandlerRead<dWidth, aShift>
	{
	public:
		using uintx_t = typename HandlerSize<dWidth>::uintx_t;

		HandlerReadDispatch(AddressSpace *space, const HandlerEntry::range &init,
			HandlerRead<dWidth, aShift> *handler = nullptr)
		: HandlerRead<dWidth, aShift>(space, 0)
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

		~HandlerReadDispatch()
		{
			// Clear all dispatch table
			for (int idx = 0; idx < count; idx++)
				dispatch[idx]->unref();
		}

		inline ctag_t *getName() { return "dispatch"; }

		uintx_t read(offs_t offset) override
		{
			offs_t off = (offset >> lowBits) & bitMask;
			assert(dispatch[off] != nullptr);
			return dispatch[off]->read(offset, data);
		}

		uintx_t read(offs_t offset, uintx_t mask) override
		{
			offs_t off = (offset >> lowBits) & bitMask;
			assert(dispatch[off] != nullptr);
			return dispatch[off]->read(offset, data, mask);
		}

	protected:
		static constexpr uint32_t lowBits  = determineDispatchLowBits(highBits, dWidth, aShift);
		static constexpr uint32_t bitCount = highBits > lowBits ? highBits - lowBits : 0;
		static constexpr uint32_t count    = 1u << bitCount;

		static constexpr offs_t   bitMask  = (1u << bitCount) - 1;

		HandlerRead<dWidth, aShift> *dispatch[count];
		HandlerEntry::range ranges[count];
	};

}
