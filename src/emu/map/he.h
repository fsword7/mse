/*
 * he.h - handler entry package
 *
 *  Created on: Jan 10, 2021
 *      Author: Tim Stark
 */

#pragma once

namespace map
{
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
}
