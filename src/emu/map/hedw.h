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

		HandlerWrite<dWidth, aShift> *const *getDispatch() const { return dispatch; }

		string getName() const override { return "dispatch"; }

		void write(offs_t offset, uintx_t data, cpuDevice *cpu) override
		{
			offs_t off = (offset >> lowBits) & bitMask;
			assert(dispatch[off] != nullptr);
			dispatch[off]->write(offset, data);
		}

		void write(offs_t offset, uintx_t data, uintx_t mask, cpuDevice *cpu) override
		{
			offs_t off = (offset >> lowBits) & bitMask;
			assert(dispatch[off] != nullptr);
			dispatch[off]->write(offset, data, mask);
		}
		
		void populateMirror(offs_t sAddr, offs_t eAddr, offs_t sOrgin, offs_t eOrgin, offs_t mirror, HandlerWrite<dWidth, aShift> *handler) override;
		void populateNoMirror(offs_t sAddr, offs_t eAddr, offs_t sOrgin, offs_t eOrgin, HandlerWrite<dWidth, aShift> *handler) override;

	protected:
		static constexpr uint32_t lowBits  = determineDispatchLowBits(highBits, dWidth, aShift);
		static constexpr uint32_t bitCount = highBits > lowBits ? highBits - lowBits : 0;
		static constexpr uint32_t count    = 1u << bitCount;

		static constexpr offs_t   bitMask  = (1u << bitCount) - 1;

		HandlerWrite<dWidth, aShift> *dispatch[count];
		HandlerEntry::range ranges[count];
	};

	template <int highBits, int dWidth, int aShift>
	void HandlerWriteDispatch<highBits, dWidth, aShift>::populateMirror(offs_t sAddr, offs_t eAddr, offs_t sOrgin, offs_t eOrgin, offs_t mirror,
		HandlerWrite<dWidth, aShift> *handler)
	{
		assert(handler != nullptr);
	}
	
	template <int highBits, int dWidth, int aShift>
	void HandlerWriteDispatch<highBits, dWidth, aShift>::populateNoMirror(offs_t sAddr, offs_t eAddr, offs_t sOrgin, offs_t eOrgin,
		HandlerWrite<dWidth, aShift> *handler)
	{
		assert(handler != nullptr);
	}

}
