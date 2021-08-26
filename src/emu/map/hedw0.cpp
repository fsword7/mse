/*
 * hedr.cpp - Read Handler Entry package - Dispatch
 *
 *  Created on: Apr 5, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/hedw.h"

using namespace aspace;

template class HandlerWriteDispatch<0, 0, 0>;
template class HandlerWriteDispatch<1, 0, 0>;
template class HandlerWriteDispatch<2, 0, 0>;
template class HandlerWriteDispatch<3, 0, 0>;
template class HandlerWriteDispatch<4, 0, 0>;
template class HandlerWriteDispatch<5, 0, 0>;
template class HandlerWriteDispatch<6, 0, 0>;
template class HandlerWriteDispatch<7, 0, 0>;
template class HandlerWriteDispatch<8, 0, 0>;

template class HandlerWriteDispatch<1, 1, 0>;
template class HandlerWriteDispatch<2, 1, 0>;
template class HandlerWriteDispatch<3, 1, 0>;
template class HandlerWriteDispatch<4, 1, 0>;
template class HandlerWriteDispatch<5, 1, 0>;
template class HandlerWriteDispatch<6, 1, 0>;
template class HandlerWriteDispatch<7, 1, 0>;
template class HandlerWriteDispatch<8, 1, 0>;

template class HandlerWriteDispatch<0, 1, 1>;
template class HandlerWriteDispatch<1, 1, 1>;
template class HandlerWriteDispatch<2, 1, 1>;
template class HandlerWriteDispatch<3, 1, 1>;
template class HandlerWriteDispatch<4, 1, 1>;
template class HandlerWriteDispatch<5, 1, 1>;
template class HandlerWriteDispatch<6, 1, 1>;
template class HandlerWriteDispatch<7, 1, 1>;
template class HandlerWriteDispatch<8, 1, 1>;

template class HandlerWriteDispatch<2, 2, 0>;
template class HandlerWriteDispatch<3, 2, 0>;
template class HandlerWriteDispatch<4, 2, 0>;
template class HandlerWriteDispatch<5, 2, 0>;
template class HandlerWriteDispatch<6, 2, 0>;
template class HandlerWriteDispatch<7, 2, 0>;
template class HandlerWriteDispatch<8, 2, 0>;

template class HandlerWriteDispatch<1, 2, 1>;
template class HandlerWriteDispatch<2, 2, 1>;
template class HandlerWriteDispatch<3, 2, 1>;
template class HandlerWriteDispatch<4, 2, 1>;
template class HandlerWriteDispatch<5, 2, 1>;
template class HandlerWriteDispatch<6, 2, 1>;
template class HandlerWriteDispatch<7, 2, 1>;
template class HandlerWriteDispatch<8, 2, 1>;

template class HandlerWriteDispatch<0, 2, 2>;
template class HandlerWriteDispatch<1, 2, 2>;
template class HandlerWriteDispatch<2, 2, 2>;
template class HandlerWriteDispatch<3, 2, 2>;
template class HandlerWriteDispatch<4, 2, 2>;
template class HandlerWriteDispatch<5, 2, 2>;
template class HandlerWriteDispatch<6, 2, 2>;
template class HandlerWriteDispatch<7, 2, 2>;
template class HandlerWriteDispatch<8, 2, 2>;

template class HandlerWriteDispatch<3, 3, 0>;
template class HandlerWriteDispatch<4, 3, 0>;
template class HandlerWriteDispatch<5, 3, 0>;
template class HandlerWriteDispatch<6, 3, 0>;
template class HandlerWriteDispatch<7, 3, 0>;
template class HandlerWriteDispatch<8, 3, 0>;

template class HandlerWriteDispatch<2, 3, 1>;
template class HandlerWriteDispatch<3, 3, 1>;
template class HandlerWriteDispatch<4, 3, 1>;
template class HandlerWriteDispatch<5, 3, 1>;
template class HandlerWriteDispatch<6, 3, 1>;
template class HandlerWriteDispatch<7, 3, 1>;
template class HandlerWriteDispatch<8, 3, 1>;

template class HandlerWriteDispatch<1, 3, 2>;
template class HandlerWriteDispatch<2, 3, 2>;
template class HandlerWriteDispatch<3, 3, 2>;
template class HandlerWriteDispatch<4, 3, 2>;
template class HandlerWriteDispatch<5, 3, 2>;
template class HandlerWriteDispatch<6, 3, 2>;
template class HandlerWriteDispatch<7, 3, 2>;
template class HandlerWriteDispatch<8, 3, 2>;

template class HandlerWriteDispatch<0, 3, 3>;
template class HandlerWriteDispatch<1, 3, 3>;
template class HandlerWriteDispatch<2, 3, 3>;
template class HandlerWriteDispatch<3, 3, 3>;
template class HandlerWriteDispatch<4, 3, 3>;
template class HandlerWriteDispatch<5, 3, 3>;
template class HandlerWriteDispatch<6, 3, 3>;
template class HandlerWriteDispatch<7, 3, 3>;
template class HandlerWriteDispatch<8, 3, 3>;
