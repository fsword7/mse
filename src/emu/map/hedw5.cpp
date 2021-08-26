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

template class HandlerWriteDispatch<41, 0, 0>;
template class HandlerWriteDispatch<42, 0, 0>;
template class HandlerWriteDispatch<43, 0, 0>;
template class HandlerWriteDispatch<44, 0, 0>;
template class HandlerWriteDispatch<45, 0, 0>;
template class HandlerWriteDispatch<46, 0, 0>;
template class HandlerWriteDispatch<47, 0, 0>;
template class HandlerWriteDispatch<48, 0, 0>;

template class HandlerWriteDispatch<41, 1, 0>;
template class HandlerWriteDispatch<42, 1, 0>;
template class HandlerWriteDispatch<43, 1, 0>;
template class HandlerWriteDispatch<44, 1, 0>;
template class HandlerWriteDispatch<45, 1, 0>;
template class HandlerWriteDispatch<46, 1, 0>;
template class HandlerWriteDispatch<47, 1, 0>;
template class HandlerWriteDispatch<48, 1, 0>;

template class HandlerWriteDispatch<41, 1, 1>;
template class HandlerWriteDispatch<42, 1, 1>;
template class HandlerWriteDispatch<43, 1, 1>;
template class HandlerWriteDispatch<44, 1, 1>;
template class HandlerWriteDispatch<45, 1, 1>;
template class HandlerWriteDispatch<46, 1, 1>;
template class HandlerWriteDispatch<47, 1, 1>;
template class HandlerWriteDispatch<48, 1, 1>;

template class HandlerWriteDispatch<41, 2, 0>;
template class HandlerWriteDispatch<42, 2, 0>;
template class HandlerWriteDispatch<43, 2, 0>;
template class HandlerWriteDispatch<44, 2, 0>;
template class HandlerWriteDispatch<45, 2, 0>;
template class HandlerWriteDispatch<46, 2, 0>;
template class HandlerWriteDispatch<47, 2, 0>;
template class HandlerWriteDispatch<48, 2, 0>;

template class HandlerWriteDispatch<41, 2, 1>;
template class HandlerWriteDispatch<42, 2, 1>;
template class HandlerWriteDispatch<43, 2, 1>;
template class HandlerWriteDispatch<44, 2, 1>;
template class HandlerWriteDispatch<45, 2, 1>;
template class HandlerWriteDispatch<46, 2, 1>;
template class HandlerWriteDispatch<47, 2, 1>;
template class HandlerWriteDispatch<48, 2, 1>;

template class HandlerWriteDispatch<41, 2, 2>;
template class HandlerWriteDispatch<42, 2, 2>;
template class HandlerWriteDispatch<43, 2, 2>;
template class HandlerWriteDispatch<44, 2, 2>;
template class HandlerWriteDispatch<45, 2, 2>;
template class HandlerWriteDispatch<46, 2, 2>;
template class HandlerWriteDispatch<47, 2, 2>;
template class HandlerWriteDispatch<48, 2, 2>;

template class HandlerWriteDispatch<41, 3, 0>;
template class HandlerWriteDispatch<42, 3, 0>;
template class HandlerWriteDispatch<43, 3, 0>;
template class HandlerWriteDispatch<44, 3, 0>;
template class HandlerWriteDispatch<45, 3, 0>;
template class HandlerWriteDispatch<46, 3, 0>;
template class HandlerWriteDispatch<47, 3, 0>;
template class HandlerWriteDispatch<48, 3, 0>;

template class HandlerWriteDispatch<41, 3, 1>;
template class HandlerWriteDispatch<42, 3, 1>;
template class HandlerWriteDispatch<43, 3, 1>;
template class HandlerWriteDispatch<44, 3, 1>;
template class HandlerWriteDispatch<45, 3, 1>;
template class HandlerWriteDispatch<46, 3, 1>;
template class HandlerWriteDispatch<47, 3, 1>;
template class HandlerWriteDispatch<48, 3, 1>;

template class HandlerWriteDispatch<41, 3, 2>;
template class HandlerWriteDispatch<42, 3, 2>;
template class HandlerWriteDispatch<43, 3, 2>;
template class HandlerWriteDispatch<44, 3, 2>;
template class HandlerWriteDispatch<45, 3, 2>;
template class HandlerWriteDispatch<46, 3, 2>;
template class HandlerWriteDispatch<47, 3, 2>;
template class HandlerWriteDispatch<48, 3, 2>;

template class HandlerWriteDispatch<41, 3, 3>;
template class HandlerWriteDispatch<42, 3, 3>;
template class HandlerWriteDispatch<43, 3, 3>;
template class HandlerWriteDispatch<44, 3, 3>;
template class HandlerWriteDispatch<45, 3, 3>;
template class HandlerWriteDispatch<46, 3, 3>;
template class HandlerWriteDispatch<47, 3, 3>;
template class HandlerWriteDispatch<48, 3, 3>;
