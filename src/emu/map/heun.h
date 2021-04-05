/*
 * heun.h - Handler entry unmapped package
 *
 *  Created on: Jan 10, 2021
 *      Author: Tim Stark
 */

#pragma once

namespace aspace
{

	template <int dWidth, int aShift, int Endian>
	class HandlerReadUnmapped : public HandlerRead<dWidth, aShift, Endian>
	{
	public:
		using uintx_t = typename HandlerRead<dWidth, aShift, Endian>::uintx_t;
		using rhe = HandlerRead<dWidth, aShift, Endian>;

		HandlerReadUnmapped(AddressSpace *space)
		: HandlerRead<dWidth, aShift, Endian>(space, 0)
		{ }

		~HandlerReadUnmapped() = default;

		inline ctag_t *getName() const { return "unmapped"; }

		uintx_t read(offs_t offset, offs_t mask)
		{
			return rhe::space->getUnmapped();
		}
	};

	template <int dWidth, int aShift, int Endian>
	class HandlerWriteUnmapped : public HandlerWrite<dWidth, aShift, Endian>
	{
	public:
		using uintx_t = typename HandlerRead<dWidth, aShift, Endian>::uintx_t;
		using whe = HandlerWrite<dWidth, aShift, Endian>;


		HandlerWriteUnmapped(AddressSpace *space)
		: HandlerWrite<dWidth, aShift, Endian>(space, 0)
		{ }

		~HandlerWriteUnmapped() = default;

		inline ctag_t *getName() const { return "unmapped"; }

		void write(offs_t offset, offs_t data)
		{

		}

		void write(offs_t offset, offs_t data, offs_t mask)
		{

		}
	};

	template <int dWidth, int aShift, int Endian>
	class HandlerReadNop : public HandlerRead<dWidth, aShift, Endian>
	{
	public:
		using uintx_t = typename HandlerRead<dWidth, aShift, Endian>::uintx_t;
		using rhe = HandlerRead<dWidth, aShift, Endian>;

		HandlerReadNop(AddressSpace *space)
		: HandlerRead<dWidth, aShift, Endian>(space, 0)
		{ }

		~HandlerReadNop() = default;

		inline ctag_t *getName() const { return "nop"; }

		uintx_t read(offs_t offset, offs_t mask)
		{
			return rhe::space->getUnmapped();
		}
	};

	template <int dWidth, int aShift, int Endian>
	class HandlerWriteNop : public HandlerWrite<dWidth, aShift, Endian>
	{
	public:
		using uintx_t = typename HandlerRead<dWidth, aShift, Endian>::uintx_t;
		using whe = HandlerWrite<dWidth, aShift, Endian>;

		HandlerWriteNop(AddressSpace *space)
		: HandlerWrite<dWidth, aShift, Endian>(space, 0)
		{ }

		~HandlerWriteNop() = default;

		inline ctag_t *getName() const { return "nop"; }

		void write(offs_t offset, offs_t data)
		{

		}

		void write(offs_t offset, offs_t data, offs_t mask)
		{

		}
	};

}
