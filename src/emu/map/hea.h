/*
 * hea.h - Handler Entry - Address space access
 *
 *  Created on: Apr 3, 2021
 *      Author: Tim Stark
 */

#pragma once

namespace aspace
{
	template <int dWidth, int aShift, int Endian>
	class HandlerReadAddress : public HandlerRead<dWidth, aShift, Endian>
	{
	public:
		using uintx_t = typename HandlerSize<dWidth>::uintx_t;

		HandlerReadAddress(AddressSpace *space, uint32_t flags)
		: HandlerRead<dWidth, aShift, Endian>(space, flags)
		{ }
		~HandlerReadAddress() = default;

		inline void setAddressSpace(offs_t base, offs_t mask)
		{
			baseAddress = base;
			maskAddress = mask;
		}

	protected:
		offs_t baseAddress = 0;
		offs_t maskAddress = 0;
	};

	template <int dWidth, int aShift, int Endian>
	class HandlerWriteAddress : public HandlerWrite<dWidth, aShift, Endian>
	{
	public:

		using uintx_t = typename HandlerSize<dWidth>::uintx_t;
		HandlerWriteAddress(AddressSpace *space, uint32_t flags)
		: HandlerWrite<dWidth, aShift, Endian>(space, flags)
		{ }
		~HandlerWriteAddress() = default;

		inline void setAddressSpace(offs_t base, offs_t mask)
		{
			baseAddress = base;
			maskAddress = mask;
		}

	protected:
		offs_t baseAddress = 0;
		offs_t maskAddress = 0;
	};

}
