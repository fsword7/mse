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

template class HandlerWriteDispatch<49, 0, 0>;
template class HandlerWriteDispatch<50, 0, 0>;
template class HandlerWriteDispatch<51, 0, 0>;
template class HandlerWriteDispatch<52, 0, 0>;
template class HandlerWriteDispatch<53, 0, 0>;
template class HandlerWriteDispatch<54, 0, 0>;
template class HandlerWriteDispatch<55, 0, 0>;
template class HandlerWriteDispatch<56, 0, 0>;

template class HandlerWriteDispatch<49, 1, 0>;
template class HandlerWriteDispatch<50, 1, 0>;
template class HandlerWriteDispatch<51, 1, 0>;
template class HandlerWriteDispatch<52, 1, 0>;
template class HandlerWriteDispatch<53, 1, 0>;
template class HandlerWriteDispatch<54, 1, 0>;
template class HandlerWriteDispatch<55, 1, 0>;
template class HandlerWriteDispatch<56, 1, 0>;

template class HandlerWriteDispatch<49, 1, 1>;
template class HandlerWriteDispatch<50, 1, 1>;
template class HandlerWriteDispatch<51, 1, 1>;
template class HandlerWriteDispatch<52, 1, 1>;
template class HandlerWriteDispatch<53, 1, 1>;
template class HandlerWriteDispatch<54, 1, 1>;
template class HandlerWriteDispatch<55, 1, 1>;
template class HandlerWriteDispatch<56, 1, 1>;

template class HandlerWriteDispatch<49, 2, 0>;
template class HandlerWriteDispatch<50, 2, 0>;
template class HandlerWriteDispatch<51, 2, 0>;
template class HandlerWriteDispatch<52, 2, 0>;
template class HandlerWriteDispatch<53, 2, 0>;
template class HandlerWriteDispatch<54, 2, 0>;
template class HandlerWriteDispatch<55, 2, 0>;
template class HandlerWriteDispatch<56, 2, 0>;

template class HandlerWriteDispatch<49, 2, 1>;
template class HandlerWriteDispatch<50, 2, 1>;
template class HandlerWriteDispatch<51, 2, 1>;
template class HandlerWriteDispatch<52, 2, 1>;
template class HandlerWriteDispatch<53, 2, 1>;
template class HandlerWriteDispatch<54, 2, 1>;
template class HandlerWriteDispatch<55, 2, 1>;
template class HandlerWriteDispatch<56, 2, 1>;

template class HandlerWriteDispatch<49, 2, 2>;
template class HandlerWriteDispatch<50, 2, 2>;
template class HandlerWriteDispatch<51, 2, 2>;
template class HandlerWriteDispatch<52, 2, 2>;
template class HandlerWriteDispatch<53, 2, 2>;
template class HandlerWriteDispatch<54, 2, 2>;
template class HandlerWriteDispatch<55, 2, 2>;
template class HandlerWriteDispatch<56, 2, 2>;

template class HandlerWriteDispatch<49, 3, 0>;
template class HandlerWriteDispatch<50, 3, 0>;
template class HandlerWriteDispatch<51, 3, 0>;
template class HandlerWriteDispatch<52, 3, 0>;
template class HandlerWriteDispatch<53, 3, 0>;
template class HandlerWriteDispatch<54, 3, 0>;
template class HandlerWriteDispatch<55, 3, 0>;
template class HandlerWriteDispatch<56, 3, 0>;

template class HandlerWriteDispatch<49, 3, 1>;
template class HandlerWriteDispatch<50, 3, 1>;
template class HandlerWriteDispatch<51, 3, 1>;
template class HandlerWriteDispatch<52, 3, 1>;
template class HandlerWriteDispatch<53, 3, 1>;
template class HandlerWriteDispatch<54, 3, 1>;
template class HandlerWriteDispatch<55, 3, 1>;
template class HandlerWriteDispatch<56, 3, 1>;

template class HandlerWriteDispatch<49, 3, 2>;
template class HandlerWriteDispatch<50, 3, 2>;
template class HandlerWriteDispatch<51, 3, 2>;
template class HandlerWriteDispatch<52, 3, 2>;
template class HandlerWriteDispatch<53, 3, 2>;
template class HandlerWriteDispatch<54, 3, 2>;
template class HandlerWriteDispatch<55, 3, 2>;
template class HandlerWriteDispatch<56, 3, 2>;

template class HandlerWriteDispatch<49, 3, 3>;
template class HandlerWriteDispatch<50, 3, 3>;
template class HandlerWriteDispatch<51, 3, 3>;
template class HandlerWriteDispatch<52, 3, 3>;
template class HandlerWriteDispatch<53, 3, 3>;
template class HandlerWriteDispatch<54, 3, 3>;
template class HandlerWriteDispatch<55, 3, 3>;
template class HandlerWriteDispatch<56, 3, 3>;
