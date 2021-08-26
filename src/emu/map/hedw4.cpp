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

template class HandlerWriteDispatch<33, 0, 0>;
template class HandlerWriteDispatch<34, 0, 0>;
template class HandlerWriteDispatch<35, 0, 0>;
template class HandlerWriteDispatch<36, 0, 0>;
template class HandlerWriteDispatch<37, 0, 0>;
template class HandlerWriteDispatch<38, 0, 0>;
template class HandlerWriteDispatch<39, 0, 0>;
template class HandlerWriteDispatch<40, 0, 0>;

template class HandlerWriteDispatch<33, 1, 0>;
template class HandlerWriteDispatch<34, 1, 0>;
template class HandlerWriteDispatch<35, 1, 0>;
template class HandlerWriteDispatch<36, 1, 0>;
template class HandlerWriteDispatch<37, 1, 0>;
template class HandlerWriteDispatch<38, 1, 0>;
template class HandlerWriteDispatch<39, 1, 0>;
template class HandlerWriteDispatch<40, 1, 0>;

template class HandlerWriteDispatch<33, 1, 1>;
template class HandlerWriteDispatch<34, 1, 1>;
template class HandlerWriteDispatch<35, 1, 1>;
template class HandlerWriteDispatch<36, 1, 1>;
template class HandlerWriteDispatch<37, 1, 1>;
template class HandlerWriteDispatch<38, 1, 1>;
template class HandlerWriteDispatch<39, 1, 1>;
template class HandlerWriteDispatch<40, 1, 1>;

template class HandlerWriteDispatch<33, 2, 0>;
template class HandlerWriteDispatch<34, 2, 0>;
template class HandlerWriteDispatch<35, 2, 0>;
template class HandlerWriteDispatch<36, 2, 0>;
template class HandlerWriteDispatch<37, 2, 0>;
template class HandlerWriteDispatch<38, 2, 0>;
template class HandlerWriteDispatch<39, 2, 0>;
template class HandlerWriteDispatch<40, 2, 0>;

template class HandlerWriteDispatch<33, 2, 1>;
template class HandlerWriteDispatch<34, 2, 1>;
template class HandlerWriteDispatch<35, 2, 1>;
template class HandlerWriteDispatch<36, 2, 1>;
template class HandlerWriteDispatch<37, 2, 1>;
template class HandlerWriteDispatch<38, 2, 1>;
template class HandlerWriteDispatch<39, 2, 1>;
template class HandlerWriteDispatch<40, 2, 1>;

template class HandlerWriteDispatch<33, 2, 2>;
template class HandlerWriteDispatch<34, 2, 2>;
template class HandlerWriteDispatch<35, 2, 2>;
template class HandlerWriteDispatch<36, 2, 2>;
template class HandlerWriteDispatch<37, 2, 2>;
template class HandlerWriteDispatch<38, 2, 2>;
template class HandlerWriteDispatch<39, 2, 2>;
template class HandlerWriteDispatch<40, 2, 2>;

template class HandlerWriteDispatch<33, 3, 0>;
template class HandlerWriteDispatch<34, 3, 0>;
template class HandlerWriteDispatch<35, 3, 0>;
template class HandlerWriteDispatch<36, 3, 0>;
template class HandlerWriteDispatch<37, 3, 0>;
template class HandlerWriteDispatch<38, 3, 0>;
template class HandlerWriteDispatch<39, 3, 0>;
template class HandlerWriteDispatch<40, 3, 0>;

template class HandlerWriteDispatch<33, 3, 1>;
template class HandlerWriteDispatch<34, 3, 1>;
template class HandlerWriteDispatch<35, 3, 1>;
template class HandlerWriteDispatch<36, 3, 1>;
template class HandlerWriteDispatch<37, 3, 1>;
template class HandlerWriteDispatch<38, 3, 1>;
template class HandlerWriteDispatch<39, 3, 1>;
template class HandlerWriteDispatch<40, 3, 1>;

template class HandlerWriteDispatch<33, 3, 2>;
template class HandlerWriteDispatch<34, 3, 2>;
template class HandlerWriteDispatch<35, 3, 2>;
template class HandlerWriteDispatch<36, 3, 2>;
template class HandlerWriteDispatch<37, 3, 2>;
template class HandlerWriteDispatch<38, 3, 2>;
template class HandlerWriteDispatch<39, 3, 2>;
template class HandlerWriteDispatch<40, 3, 2>;

template class HandlerWriteDispatch<33, 3, 3>;
template class HandlerWriteDispatch<34, 3, 3>;
template class HandlerWriteDispatch<35, 3, 3>;
template class HandlerWriteDispatch<36, 3, 3>;
template class HandlerWriteDispatch<37, 3, 3>;
template class HandlerWriteDispatch<38, 3, 3>;
template class HandlerWriteDispatch<39, 3, 3>;
template class HandlerWriteDispatch<40, 3, 3>;
