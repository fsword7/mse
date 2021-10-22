/*
 * devcall.cpp - device callback delegate package
 *
 *  Created on: Oct 20, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/devcall.h"

template class cbRead_t<uint8_t>;
template class cbRead_t<uint16_t>;
template class cbRead_t<uint32_t>;
template class cbRead_t<uint64_t>;

template class cbWrite_t<uint8_t>;
template class cbWrite_t<uint16_t>;
template class cbWrite_t<uint32_t>;
template class cbWrite_t<uint64_t>;

