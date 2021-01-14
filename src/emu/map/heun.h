/*
 * heun.h - Handler entry unmapped package
 *
 *  Created on: Jan 10, 2021
 *      Author: Tim Stark
 */

#pragma once

namespace map
{

	template <int dWidth, int aShift, int Endian>
	class HandlerReadUnmapped : public HandlerRead<dWidth, aShift, Endian>
	{

	};

	template <int dWidth, int aShift, int Endian>
	class HandlerWriteUnmapped : public HandlerWrite<dWidth, aShift, Endian>
	{

	};

	template <int dWidth, int aShift, int Endian>
	class HandlerReadNop : public HandlerRead<dWidth, aShift, Endian>
	{

	};

	template <int dWidth, int aShift, int Endian>
	class HandlerWriteNop : public HandlerWrite<dWidth, aShift, Endian>
	{

	};

}
