/*
 * heun.cpp - Handler Entry - Unmapped/NOP
 *
 *  Created on: Jan 10, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/map.h"

using namespace aspace;

template class HandlerReadUnmapped<0, 0>;
template class HandlerReadUnmapped<1, 0>;
template class HandlerReadUnmapped<1, 1>;
template class HandlerReadUnmapped<2, 0>;
template class HandlerReadUnmapped<2, 1>;
template class HandlerReadUnmapped<2, 2>;
template class HandlerReadUnmapped<3, 0>;
template class HandlerReadUnmapped<3, 1>;
template class HandlerReadUnmapped<3, 2>;
template class HandlerReadUnmapped<3, 3>;

template class HandlerWriteUnmapped<0, 0>;
template class HandlerWriteUnmapped<1, 0>;
template class HandlerWriteUnmapped<1, 1>;
template class HandlerWriteUnmapped<2, 0>;
template class HandlerWriteUnmapped<2, 1>;
template class HandlerWriteUnmapped<2, 2>;
template class HandlerWriteUnmapped<3, 0>;
template class HandlerWriteUnmapped<3, 1>;
template class HandlerWriteUnmapped<3, 2>;
template class HandlerWriteUnmapped<3, 3>;

template class HandlerReadNop<0, 0>;
template class HandlerReadNop<1, 0>;
template class HandlerReadNop<1, 1>;
template class HandlerReadNop<2, 0>;
template class HandlerReadNop<2, 1>;
template class HandlerReadNop<2, 2>;
template class HandlerReadNop<3, 0>;
template class HandlerReadNop<3, 1>;
template class HandlerReadNop<3, 2>;
template class HandlerReadNop<3, 3>;

template class HandlerWriteNop<0, 0>;
template class HandlerWriteNop<1, 0>;
template class HandlerWriteNop<1, 1>;
template class HandlerWriteNop<2, 0>;
template class HandlerWriteNop<2, 1>;
template class HandlerWriteNop<2, 2>;
template class HandlerWriteNop<3, 0>;
template class HandlerWriteNop<3, 1>;
template class HandlerWriteNop<3, 2>;
template class HandlerWriteNop<3, 3>;
