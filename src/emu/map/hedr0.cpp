/*
 * hedr.cpp - Read Handler Entry package - Dispatch
 *
 *  Created on: Apr 5, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/map/hedr.h"

using namespace aspace;

template class HandlerReadDispatch<0, 0, 0>;
template class HandlerReadDispatch<1, 0, 0>;
template class HandlerReadDispatch<2, 0, 0>;
template class HandlerReadDispatch<3, 0, 0>;
template class HandlerReadDispatch<4, 0, 0>;
template class HandlerReadDispatch<5, 0, 0>;
template class HandlerReadDispatch<6, 0, 0>;
template class HandlerReadDispatch<7, 0, 0>;
template class HandlerReadDispatch<8, 0, 0>;

template class HandlerReadDispatch<1, 1, 0>;
template class HandlerReadDispatch<2, 1, 0>;
template class HandlerReadDispatch<3, 1, 0>;
template class HandlerReadDispatch<4, 1, 0>;
template class HandlerReadDispatch<5, 1, 0>;
template class HandlerReadDispatch<6, 1, 0>;
template class HandlerReadDispatch<7, 1, 0>;
template class HandlerReadDispatch<8, 1, 0>;

template class HandlerReadDispatch<0, 1, 1>;
template class HandlerReadDispatch<1, 1, 1>;
template class HandlerReadDispatch<2, 1, 1>;
template class HandlerReadDispatch<3, 1, 1>;
template class HandlerReadDispatch<4, 1, 1>;
template class HandlerReadDispatch<5, 1, 1>;
template class HandlerReadDispatch<6, 1, 1>;
template class HandlerReadDispatch<7, 1, 1>;
template class HandlerReadDispatch<8, 1, 1>;

template class HandlerReadDispatch<2, 2, 0>;
template class HandlerReadDispatch<3, 2, 0>;
template class HandlerReadDispatch<4, 2, 0>;
template class HandlerReadDispatch<5, 2, 0>;
template class HandlerReadDispatch<6, 2, 0>;
template class HandlerReadDispatch<7, 2, 0>;
template class HandlerReadDispatch<8, 2, 0>;

template class HandlerReadDispatch<1, 2, 1>;
template class HandlerReadDispatch<2, 2, 1>;
template class HandlerReadDispatch<3, 2, 1>;
template class HandlerReadDispatch<4, 2, 1>;
template class HandlerReadDispatch<5, 2, 1>;
template class HandlerReadDispatch<6, 2, 1>;
template class HandlerReadDispatch<7, 2, 1>;
template class HandlerReadDispatch<8, 2, 1>;

template class HandlerReadDispatch<0, 2, 2>;
template class HandlerReadDispatch<1, 2, 2>;
template class HandlerReadDispatch<2, 2, 2>;
template class HandlerReadDispatch<3, 2, 2>;
template class HandlerReadDispatch<4, 2, 2>;
template class HandlerReadDispatch<5, 2, 2>;
template class HandlerReadDispatch<6, 2, 2>;
template class HandlerReadDispatch<7, 2, 2>;
template class HandlerReadDispatch<8, 2, 2>;

template class HandlerReadDispatch<3, 3, 0>;
template class HandlerReadDispatch<4, 3, 0>;
template class HandlerReadDispatch<5, 3, 0>;
template class HandlerReadDispatch<6, 3, 0>;
template class HandlerReadDispatch<7, 3, 0>;
template class HandlerReadDispatch<8, 3, 0>;

template class HandlerReadDispatch<2, 3, 1>;
template class HandlerReadDispatch<3, 3, 1>;
template class HandlerReadDispatch<4, 3, 1>;
template class HandlerReadDispatch<5, 3, 1>;
template class HandlerReadDispatch<6, 3, 1>;
template class HandlerReadDispatch<7, 3, 1>;
template class HandlerReadDispatch<8, 3, 1>;

template class HandlerReadDispatch<1, 3, 2>;
template class HandlerReadDispatch<2, 3, 2>;
template class HandlerReadDispatch<3, 3, 2>;
template class HandlerReadDispatch<4, 3, 2>;
template class HandlerReadDispatch<5, 3, 2>;
template class HandlerReadDispatch<6, 3, 2>;
template class HandlerReadDispatch<7, 3, 2>;
template class HandlerReadDispatch<8, 3, 2>;

template class HandlerReadDispatch<0, 3, 3>;
template class HandlerReadDispatch<1, 3, 3>;
template class HandlerReadDispatch<2, 3, 3>;
template class HandlerReadDispatch<3, 3, 3>;
template class HandlerReadDispatch<4, 3, 3>;
template class HandlerReadDispatch<5, 3, 3>;
template class HandlerReadDispatch<6, 3, 3>;
template class HandlerReadDispatch<7, 3, 3>;
template class HandlerReadDispatch<8, 3, 3>;
