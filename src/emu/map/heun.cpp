/*
 * heun.cpp - Handler Entry - Unmapped/NOP
 *
 *  Created on: Jan 10, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/map.h"

using namespace aspace;

template class HandlerReadUnmapped<0, 0, LittleEndian>;
template class HandlerReadUnmapped<0, 0, BigEndian>;
template class HandlerReadUnmapped<1, 0, LittleEndian>;
template class HandlerReadUnmapped<1, 0, BigEndian>;
template class HandlerReadUnmapped<1, 1, LittleEndian>;
template class HandlerReadUnmapped<1, 1, BigEndian>;
template class HandlerReadUnmapped<2, 0, LittleEndian>;
template class HandlerReadUnmapped<2, 0, BigEndian>;
template class HandlerReadUnmapped<2, 1, LittleEndian>;
template class HandlerReadUnmapped<2, 1, BigEndian>;
template class HandlerReadUnmapped<2, 2, LittleEndian>;
template class HandlerReadUnmapped<2, 2, BigEndian>;
template class HandlerReadUnmapped<3, 0, LittleEndian>;
template class HandlerReadUnmapped<3, 0, BigEndian>;
template class HandlerReadUnmapped<3, 1, LittleEndian>;
template class HandlerReadUnmapped<3, 1, BigEndian>;
template class HandlerReadUnmapped<3, 2, LittleEndian>;
template class HandlerReadUnmapped<3, 2, BigEndian>;
template class HandlerReadUnmapped<3, 3, LittleEndian>;
template class HandlerReadUnmapped<3, 3, BigEndian>;

template class HandlerWriteUnmapped<0, 0, LittleEndian>;
template class HandlerWriteUnmapped<0, 0, BigEndian>;
template class HandlerWriteUnmapped<1, 0, LittleEndian>;
template class HandlerWriteUnmapped<1, 0, BigEndian>;
template class HandlerWriteUnmapped<1, 1, LittleEndian>;
template class HandlerWriteUnmapped<1, 1, BigEndian>;
template class HandlerWriteUnmapped<2, 0, LittleEndian>;
template class HandlerWriteUnmapped<2, 0, BigEndian>;
template class HandlerWriteUnmapped<2, 1, LittleEndian>;
template class HandlerWriteUnmapped<2, 1, BigEndian>;
template class HandlerWriteUnmapped<2, 2, LittleEndian>;
template class HandlerWriteUnmapped<2, 2, BigEndian>;
template class HandlerWriteUnmapped<3, 0, LittleEndian>;
template class HandlerWriteUnmapped<3, 0, BigEndian>;
template class HandlerWriteUnmapped<3, 1, LittleEndian>;
template class HandlerWriteUnmapped<3, 1, BigEndian>;
template class HandlerWriteUnmapped<3, 2, LittleEndian>;
template class HandlerWriteUnmapped<3, 2, BigEndian>;
template class HandlerWriteUnmapped<3, 3, LittleEndian>;
template class HandlerWriteUnmapped<3, 3, BigEndian>;

template class HandlerReadNop<0, 0, LittleEndian>;
template class HandlerReadNop<0, 0, BigEndian>;
template class HandlerReadNop<1, 0, LittleEndian>;
template class HandlerReadNop<1, 0, BigEndian>;
template class HandlerReadNop<1, 1, LittleEndian>;
template class HandlerReadNop<1, 1, BigEndian>;
template class HandlerReadNop<2, 0, LittleEndian>;
template class HandlerReadNop<2, 0, BigEndian>;
template class HandlerReadNop<2, 1, LittleEndian>;
template class HandlerReadNop<2, 1, BigEndian>;
template class HandlerReadNop<2, 2, LittleEndian>;
template class HandlerReadNop<2, 2, BigEndian>;
template class HandlerReadNop<3, 0, LittleEndian>;
template class HandlerReadNop<3, 0, BigEndian>;
template class HandlerReadNop<3, 1, LittleEndian>;
template class HandlerReadNop<3, 1, BigEndian>;
template class HandlerReadNop<3, 2, LittleEndian>;
template class HandlerReadNop<3, 2, BigEndian>;
template class HandlerReadNop<3, 3, LittleEndian>;
template class HandlerReadNop<3, 3, BigEndian>;

template class HandlerWriteNop<0, 0, LittleEndian>;
template class HandlerWriteNop<0, 0, BigEndian>;
template class HandlerWriteNop<1, 0, LittleEndian>;
template class HandlerWriteNop<1, 0, BigEndian>;
template class HandlerWriteNop<1, 1, LittleEndian>;
template class HandlerWriteNop<1, 1, BigEndian>;
template class HandlerWriteNop<2, 0, LittleEndian>;
template class HandlerWriteNop<2, 0, BigEndian>;
template class HandlerWriteNop<2, 1, LittleEndian>;
template class HandlerWriteNop<2, 1, BigEndian>;
template class HandlerWriteNop<2, 2, LittleEndian>;
template class HandlerWriteNop<2, 2, BigEndian>;
template class HandlerWriteNop<3, 0, LittleEndian>;
template class HandlerWriteNop<3, 0, BigEndian>;
template class HandlerWriteNop<3, 1, LittleEndian>;
template class HandlerWriteNop<3, 1, BigEndian>;
template class HandlerWriteNop<3, 2, LittleEndian>;
template class HandlerWriteNop<3, 2, BigEndian>;
template class HandlerWriteNop<3, 3, LittleEndian>;
template class HandlerWriteNop<3, 3, BigEndian>;
