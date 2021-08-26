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

template class HandlerWriteDispatch<25, 0, 0>;
template class HandlerWriteDispatch<26, 0, 0>;
template class HandlerWriteDispatch<27, 0, 0>;
template class HandlerWriteDispatch<28, 0, 0>;
template class HandlerWriteDispatch<29, 0, 0>;
template class HandlerWriteDispatch<30, 0, 0>;
template class HandlerWriteDispatch<31, 0, 0>;
template class HandlerWriteDispatch<32, 0, 0>;

template class HandlerWriteDispatch<25, 1, 0>;
template class HandlerWriteDispatch<26, 1, 0>;
template class HandlerWriteDispatch<27, 1, 0>;
template class HandlerWriteDispatch<28, 1, 0>;
template class HandlerWriteDispatch<29, 1, 0>;
template class HandlerWriteDispatch<30, 1, 0>;
template class HandlerWriteDispatch<31, 1, 0>;
template class HandlerWriteDispatch<32, 1, 0>;

template class HandlerWriteDispatch<25, 1, 1>;
template class HandlerWriteDispatch<26, 1, 1>;
template class HandlerWriteDispatch<27, 1, 1>;
template class HandlerWriteDispatch<28, 1, 1>;
template class HandlerWriteDispatch<29, 1, 1>;
template class HandlerWriteDispatch<30, 1, 1>;
template class HandlerWriteDispatch<31, 1, 1>;
template class HandlerWriteDispatch<32, 1, 1>;

template class HandlerWriteDispatch<25, 2, 0>;
template class HandlerWriteDispatch<26, 2, 0>;
template class HandlerWriteDispatch<27, 2, 0>;
template class HandlerWriteDispatch<28, 2, 0>;
template class HandlerWriteDispatch<29, 2, 0>;
template class HandlerWriteDispatch<30, 2, 0>;
template class HandlerWriteDispatch<31, 2, 0>;
template class HandlerWriteDispatch<32, 2, 0>;

template class HandlerWriteDispatch<25, 2, 1>;
template class HandlerWriteDispatch<26, 2, 1>;
template class HandlerWriteDispatch<27, 2, 1>;
template class HandlerWriteDispatch<28, 2, 1>;
template class HandlerWriteDispatch<29, 2, 1>;
template class HandlerWriteDispatch<30, 2, 1>;
template class HandlerWriteDispatch<31, 2, 1>;
template class HandlerWriteDispatch<32, 2, 1>;

template class HandlerWriteDispatch<25, 2, 2>;
template class HandlerWriteDispatch<26, 2, 2>;
template class HandlerWriteDispatch<27, 2, 2>;
template class HandlerWriteDispatch<28, 2, 2>;
template class HandlerWriteDispatch<29, 2, 2>;
template class HandlerWriteDispatch<30, 2, 2>;
template class HandlerWriteDispatch<31, 2, 2>;
template class HandlerWriteDispatch<32, 2, 2>;

template class HandlerWriteDispatch<25, 3, 0>;
template class HandlerWriteDispatch<26, 3, 0>;
template class HandlerWriteDispatch<27, 3, 0>;
template class HandlerWriteDispatch<28, 3, 0>;
template class HandlerWriteDispatch<29, 3, 0>;
template class HandlerWriteDispatch<30, 3, 0>;
template class HandlerWriteDispatch<31, 3, 0>;
template class HandlerWriteDispatch<32, 3, 0>;

template class HandlerWriteDispatch<25, 3, 1>;
template class HandlerWriteDispatch<26, 3, 1>;
template class HandlerWriteDispatch<27, 3, 1>;
template class HandlerWriteDispatch<28, 3, 1>;
template class HandlerWriteDispatch<29, 3, 1>;
template class HandlerWriteDispatch<30, 3, 1>;
template class HandlerWriteDispatch<31, 3, 1>;
template class HandlerWriteDispatch<32, 3, 1>;

template class HandlerWriteDispatch<25, 3, 2>;
template class HandlerWriteDispatch<26, 3, 2>;
template class HandlerWriteDispatch<27, 3, 2>;
template class HandlerWriteDispatch<28, 3, 2>;
template class HandlerWriteDispatch<29, 3, 2>;
template class HandlerWriteDispatch<30, 3, 2>;
template class HandlerWriteDispatch<31, 3, 2>;
template class HandlerWriteDispatch<32, 3, 2>;

template class HandlerWriteDispatch<25, 3, 3>;
template class HandlerWriteDispatch<26, 3, 3>;
template class HandlerWriteDispatch<27, 3, 3>;
template class HandlerWriteDispatch<28, 3, 3>;
template class HandlerWriteDispatch<29, 3, 3>;
template class HandlerWriteDispatch<30, 3, 3>;
template class HandlerWriteDispatch<31, 3, 3>;
template class HandlerWriteDispatch<32, 3, 3>;
