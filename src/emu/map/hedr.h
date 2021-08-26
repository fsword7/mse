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

		HandlerRead<dWidth, aShift> *const *getDispatch() const { return dispatch; }

		string getName() const override { return "dispatch"; }

		uintx_t read(offs_t offset, cpuDevice *cpu) override
		{
			offs_t off = (offset >> lowBits) & bitMask;
			assert(dispatch[off] != nullptr);
			return dispatch[off]->read(offset);
		}

		uintx_t read(offs_t offset, uintx_t mask, cpuDevice *cpu) override
		{
			offs_t off = (offset >> lowBits) & bitMask;
			assert(dispatch[off] != nullptr);
			return dispatch[off]->read(offset, mask);
		}

		void populateMirror(offs_t sAddr, offs_t eAddr, offs_t sOrgin, offs_t eOrgin, offs_t mirror, HandlerRead<dWidth, aShift> *handler) override;
		void populateNoMirror(offs_t sAddr, offs_t eAddr, offs_t sOrgin, offs_t eOrgin,  HandlerRead<dWidth, aShift> *handler) override;

	protected:
		static constexpr int      level    = determineDispatchLevel(highBits);
		static constexpr uint32_t lowBits  = determineDispatchLowBits(highBits, dWidth, aShift);
		static constexpr uint32_t bitCount = highBits > lowBits ? highBits - lowBits : 0;
		static constexpr uint32_t count    = 1u << bitCount;

		// static constexpr offs_t   bitMask  = (1u << bitCount) - 1;
		static constexpr offs_t   bitMask  = makeBitmask<offs_t>(bitCount);
		static constexpr offs_t   lowMask  = makeBitmask<offs_t>(lowBits);
		static constexpr offs_t   highMask = makeBitmask<offs_t>(highBits) ^ lowMask;
		static constexpr offs_t   upMask   = ~makeBitmask<offs_t>(highBits);

		HandlerRead<dWidth, aShift> *dispatch[count];
		HandlerEntry::range ranges[count];
	};

	
	template <int highBits, int dWidth, int aShift>
	void HandlerReadDispatch<highBits, dWidth, aShift>::populateNoMirror(offs_t sAddr, offs_t eAddr, offs_t sOrgin, offs_t eOrgin,
		HandlerRead<dWidth, aShift> *handler)
	{
		offs_t sEntry = (sAddr & highMask) >> lowBits;
		offs_t eEntry = (eAddr & highMask) >> lowBits;

		assert(handler != nullptr);
	}

	template <int highBits, int dWidth, int aShift>
	void HandlerReadDispatch<highBits, dWidth, aShift>::populateMirror(offs_t sAddr, offs_t eAddr, offs_t sOrgin, offs_t eOrgin, offs_t mirror,
		HandlerRead<dWidth, aShift> *handler)
	{
		offs_t highMirror = mirror & highMask;
		offs_t lowMirror = mirror & lowMask;

		assert(handler != nullptr);

		// if (lowMirror != 0)
		// {

		// }
		// else
		// {
		// 	offs_t add = ~highMirror + 1;
		// 	offs_t off = 0;

		// 	do
		// 	{
		// 		if (off != 0)
		// 			handler->ref();
		// 		populateNoMirror(sAddr | off, eAddr | off, sOrgin | off, eOrgin | off, handler);
		// 		off = (off + add) & highMirror;
		// 	}
		// 	while (off != 0);
		// }
	}

}
