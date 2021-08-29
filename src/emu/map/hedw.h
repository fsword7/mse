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
		: HandlerWrite<dWidth, aShift>(space, HandlerEntry::heDispatch)
		{
			// Initialize dispatch table
			if (handler == nullptr)
				handler = space->getUnmappedWrite<dWidth, aShift>();
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

		HandlerWrite<dWidth, aShift> *const *getDispatch() const override { return dispatch; }

		string getName() const override { return "dispatch"; }

		void write(offs_t offset, uintx_t data, cpuDevice *cpu) const override
		{
			offs_t off = (offset >> lowBits) & bitMask;
			assert(dispatch[off] != nullptr);
			dispatch[off]->write(offset, data, cpu);
		}

		void write(offs_t offset, uintx_t data, uintx_t mask, cpuDevice *cpu) const override
		{
			offs_t off = (offset >> lowBits) & bitMask;
			assert(dispatch[off] != nullptr);
			dispatch[off]->write(offset, data, mask, cpu);
		}

		void dumpMap(vector<mapEntry> &map)
		{
			offs_t cur = map.empty() ? 0 : map.back().eAddr + 1;
			offs_t base = cur & upMask;
			
			do
			{
				offs_t entry = (cur >> lowBits) & bitMask;
				if (dispatch[entry]->isDispatch())
					dispatch[entry]->dumpMap(map);
				else
					map.emplace_back(mapEntry{ ranges[entry].start, ranges[entry].end, dispatch[entry] });
				cur = map.back().eAddr + 1;
			}
			while (cur && !((cur ^ base) & upMask));
		}

		void cutRangeBefore(offs_t addr, int start = count);
		void cutRangeAfter(offs_t addr, int start = -1);

		void populateSubdispatchMirror(offs_t entry, offs_t sAddr, offs_t eAddr, offs_t mAddr, offs_t sOrgin, offs_t eOrgin, HandlerWrite<dWidth, aShift> *handler) override;
		void populateMirror(offs_t sAddr, offs_t eAddr, offs_t mAddr, offs_t sOrgin, offs_t eOrgin, HandlerWrite<dWidth, aShift> *handler) override;
		void populateSubdispatchNoMirror(offs_t entry, offs_t sAddr, offs_t eAddr, offs_t sOrgin, offs_t eOrgin,  HandlerWrite<dWidth, aShift> *handler) override;
		void populateNoMirror(offs_t sAddr, offs_t eAddr, offs_t sOrgin, offs_t eOrgin,  HandlerWrite<dWidth, aShift> *handler) override;

	protected:
		static constexpr uint32_t lowBits  = determineDispatchLowBits(highBits, dWidth, aShift);
		static constexpr uint32_t bitCount = highBits > lowBits ? highBits - lowBits : 0;
		static constexpr uint32_t count    = 1u << bitCount;

		static constexpr offs_t   bitMask  = makeBitmask<offs_t>(bitCount);
		static constexpr offs_t   lowMask  = makeBitmask<offs_t>(lowBits);
		static constexpr offs_t   highMask = makeBitmask<offs_t>(highBits) ^ lowMask;
		static constexpr offs_t   upMask   = ~makeBitmask<offs_t>(highBits);


		HandlerWrite<dWidth, aShift> *dispatch[count];
		HandlerEntry::range ranges[count];
	};

	template <int highBits, int dWidth, int aShift>
	void HandlerWriteDispatch<highBits, dWidth, aShift>::cutRangeBefore(offs_t addr, int start)
	{
		while (--start >= 0 && dispatch[start] != nullptr)
		{
			if (lowBits > -aShift && dispatch[start]->isDispatch())
			{
				static_cast<HandlerWriteDispatch<highBits, dWidth, aShift> *>(dispatch[start])->cutRangeBefore(addr);
				break;
			}
			if (ranges[start].end <= addr)
				break;
			ranges[start].end = addr;
		}
	}

	template <int highBits, int dWidth, int aShift>
	void HandlerWriteDispatch<highBits, dWidth, aShift>::cutRangeAfter(offs_t addr, int start)
	{
		while (++start < count && dispatch[start] != nullptr)
		{
			if (lowBits > -aShift && dispatch[start]->isDispatch())
			{
				static_cast<HandlerWriteDispatch<highBits, dWidth, aShift> *>(dispatch[start])->cutRangeAfter(addr);
				break;
			}
			if (ranges[start].end <= addr)
				break;
			ranges[start].end = addr;
		}
	}
	
	template <int highBits, int dWidth, int aShift>
	void HandlerWriteDispatch<highBits, dWidth, aShift>::populateSubdispatchNoMirror(offs_t entry, offs_t sAddr, offs_t eAddr,
		offs_t sOrgin, offs_t eOrgin, HandlerWrite<dWidth, aShift> *handler)
	{
		assert(handler != nullptr);
		auto cur = dispatch[entry];
		if (cur->isDispatch())
			cur->populateNoMirror(sAddr, eAddr, sOrgin, eOrgin, handler);
		else
		{
			auto subdispatch = new HandlerWriteDispatch<lowBits, dWidth, aShift>(this->space, ranges[entry], cur);
			cur->unref();
			dispatch[entry] = subdispatch;
			subdispatch->populateNoMirror(sAddr, eAddr, sOrgin, eOrgin, handler);
		}
	}
	
	template <int highBits, int dWidth, int aShift>
	void HandlerWriteDispatch<highBits, dWidth, aShift>::populateNoMirror(offs_t sAddr, offs_t eAddr,
		offs_t sOrgin, offs_t eOrgin, HandlerWrite<dWidth, aShift> *handler)
	{
		offs_t sEntry = (sAddr & highMask) >> lowBits;
		offs_t eEntry = (eAddr & highMask) >> lowBits;

		assert(handler != nullptr);
		cutRangeBefore(sOrgin-1, sEntry);
		cutRangeAfter(eOrgin+1, eEntry);

		if (lowBits <= dWidth + aShift)
		{
			handler->ref(eEntry - sEntry);
			for (offs_t ent = sEntry; ent <= eEntry; ent++)
			{
				dispatch[ent]->unref();
				dispatch[ent] = handler;
				ranges[ent].set(sOrgin, eOrgin);
			}
		}
		else if (sEntry == eEntry)
		{
			if (!(sAddr & lowMask) && (eAddr & lowMask) == lowMask)
			{
				dispatch[sEntry]->unref();
				dispatch[sEntry] = handler;
				ranges[sEntry].set(sOrgin, eOrgin);
			}
			else
				populateSubdispatchNoMirror(sEntry, sAddr & lowMask, eAddr & lowMask, sOrgin, eOrgin, handler);
		}
		else
		{
			if (sAddr & lowMask)
			{
				populateSubdispatchNoMirror(sEntry, sAddr & lowMask, lowMask, sOrgin, eOrgin, handler);
				sEntry++;
				if (sEntry <= eEntry)
					handler->ref();
			}

			if ((eAddr & lowMask) != lowMask)
			{
				populateSubdispatchNoMirror(eEntry, 0, eAddr & lowMask, sOrgin, eOrgin, handler);
				eEntry--;
				if (sEntry <= eEntry)
					handler->ref();
			}

			if (sEntry <= eEntry)
			{
				handler->ref(eEntry - sEntry);
				for (offs_t ent = sEntry; ent <= eEntry; ent++)
				{
					dispatch[ent]->unref();
					dispatch[ent] = handler;
					ranges[ent].set(sOrgin, eOrgin);
				}
			}
		}
	}

	template <int highBits, int dWidth, int aShift>
	void HandlerWriteDispatch<highBits, dWidth, aShift>::populateSubdispatchMirror(offs_t entry, offs_t sAddr, offs_t eAddr, offs_t mAddr,
		offs_t sOrgin, offs_t eOrgin, HandlerWrite<dWidth, aShift> *handler)
	{
		assert(handler != nullptr);
		auto cur = dispatch[entry];
		if (cur->isDispatch())
			cur->populateMirror(sAddr, eAddr, mAddr, sOrgin, eOrgin, handler);
		else
		{
			auto subdispatch = new HandlerWriteDispatch<lowBits, dWidth, aShift>(this->space, ranges[entry], cur);
			cur->unref();
			dispatch[entry] = subdispatch;
			subdispatch->populateMirror(sAddr, eAddr, mAddr, sOrgin, eOrgin, handler);
		}

	}

	template <int highBits, int dWidth, int aShift>
	void HandlerWriteDispatch<highBits, dWidth, aShift>::populateMirror(offs_t sAddr, offs_t eAddr, offs_t mAddr,
		offs_t sOrgin, offs_t eOrgin, HandlerWrite<dWidth, aShift> *handler)
	{
		offs_t highMirror = mAddr & highMask;
		offs_t lowMirror = mAddr & lowMask;

		assert(handler != nullptr);

		if (lowMirror != 0)
		{
			offs_t add = ~highMirror + 1;
			offs_t offset = 0;
			offs_t base = sAddr >> lowBits;
			sAddr &= lowMask;
			eAddr &= lowMask;
			do
			{
				if (offset != 0)
					handler->ref();
				populateSubdispatchMirror(base | (offset >> lowBits), sAddr, eAddr, lowMirror, sOrgin|offset, eOrgin|offset, handler);
				offset = (offset + add) & lowMirror;
			} while (offset != 0);
		}
		else
		{
			offs_t add = ~highMirror + 1;
			offs_t offset = 0;
			do
			{
				if (offset != 0)
					handler->ref();
				populateNoMirror(sAddr | offset, eAddr | offset, sOrgin | offset, eOrgin | offset, handler);
				offset = (offset + add) & highMirror;
			} while (offset != 0);
		}
	}

}
