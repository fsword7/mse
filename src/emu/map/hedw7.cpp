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

template class HandlerWriteDispatch<57, 0, 0>;
template class HandlerWriteDispatch<58, 0, 0>;
template class HandlerWriteDispatch<59, 0, 0>;
template class HandlerWriteDispatch<60, 0, 0>;
template class HandlerWriteDispatch<61, 0, 0>;
template class HandlerWriteDispatch<62, 0, 0>;
template class HandlerWriteDispatch<63, 0, 0>;
template class HandlerWriteDispatch<64, 0, 0>;

template class HandlerWriteDispatch<57, 1, 0>;
template class HandlerWriteDispatch<58, 1, 0>;
template class HandlerWriteDispatch<59, 1, 0>;
template class HandlerWriteDispatch<60, 1, 0>;
template class HandlerWriteDispatch<61, 1, 0>;
template class HandlerWriteDispatch<62, 1, 0>;
template class HandlerWriteDispatch<63, 1, 0>;
template class HandlerWriteDispatch<64, 1, 0>;

template class HandlerWriteDispatch<57, 1, 1>;
template class HandlerWriteDispatch<58, 1, 1>;
template class HandlerWriteDispatch<59, 1, 1>;
template class HandlerWriteDispatch<60, 1, 1>;
template class HandlerWriteDispatch<61, 1, 1>;
template class HandlerWriteDispatch<62, 1, 1>;
template class HandlerWriteDispatch<63, 1, 1>;
template class HandlerWriteDispatch<64, 1, 1>;

template class HandlerWriteDispatch<57, 2, 0>;
template class HandlerWriteDispatch<58, 2, 0>;
template class HandlerWriteDispatch<59, 2, 0>;
template class HandlerWriteDispatch<60, 2, 0>;
template class HandlerWriteDispatch<61, 2, 0>;
template class HandlerWriteDispatch<62, 2, 0>;
template class HandlerWriteDispatch<63, 2, 0>;
template class HandlerWriteDispatch<64, 2, 0>;

template class HandlerWriteDispatch<57, 2, 1>;
template class HandlerWriteDispatch<58, 2, 1>;
template class HandlerWriteDispatch<59, 2, 1>;
template class HandlerWriteDispatch<60, 2, 1>;
template class HandlerWriteDispatch<61, 2, 1>;
template class HandlerWriteDispatch<62, 2, 1>;
template class HandlerWriteDispatch<63, 2, 1>;
template class HandlerWriteDispatch<64, 2, 1>;

template class HandlerWriteDispatch<57, 2, 2>;
template class HandlerWriteDispatch<58, 2, 2>;
template class HandlerWriteDispatch<59, 2, 2>;
template class HandlerWriteDispatch<60, 2, 2>;
template class HandlerWriteDispatch<61, 2, 2>;
template class HandlerWriteDispatch<62, 2, 2>;
template class HandlerWriteDispatch<63, 2, 2>;
template class HandlerWriteDispatch<64, 2, 2>;

template class HandlerWriteDispatch<57, 3, 0>;
template class HandlerWriteDispatch<58, 3, 0>;
template class HandlerWriteDispatch<59, 3, 0>;
template class HandlerWriteDispatch<60, 3, 0>;
template class HandlerWriteDispatch<61, 3, 0>;
template class HandlerWriteDispatch<62, 3, 0>;
template class HandlerWriteDispatch<63, 3, 0>;
template class HandlerWriteDispatch<64, 3, 0>;

template class HandlerWriteDispatch<57, 3, 1>;
template class HandlerWriteDispatch<58, 3, 1>;
template class HandlerWriteDispatch<59, 3, 1>;
template class HandlerWriteDispatch<60, 3, 1>;
template class HandlerWriteDispatch<61, 3, 1>;
template class HandlerWriteDispatch<62, 3, 1>;
template class HandlerWriteDispatch<63, 3, 1>;
template class HandlerWriteDispatch<64, 3, 1>;

template class HandlerWriteDispatch<57, 3, 2>;
template class HandlerWriteDispatch<58, 3, 2>;
template class HandlerWriteDispatch<59, 3, 2>;
template class HandlerWriteDispatch<60, 3, 2>;
template class HandlerWriteDispatch<61, 3, 2>;
template class HandlerWriteDispatch<62, 3, 2>;
template class HandlerWriteDispatch<63, 3, 2>;
template class HandlerWriteDispatch<64, 3, 2>;

template class HandlerWriteDispatch<57, 3, 3>;
template class HandlerWriteDispatch<58, 3, 3>;
template class HandlerWriteDispatch<59, 3, 3>;
template class HandlerWriteDispatch<60, 3, 3>;
template class HandlerWriteDispatch<61, 3, 3>;
template class HandlerWriteDispatch<62, 3, 3>;
template class HandlerWriteDispatch<63, 3, 3>;
template class HandlerWriteDispatch<64, 3, 3>;
