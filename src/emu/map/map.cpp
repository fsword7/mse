/*
 * map.cpp - Mapping system bus package
 *
 *  Created on: Dec 12, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/addrmap.h"
#include "emu/map/map.h"

mapAddressConfig::mapAddressConfig(ctag_t *tagName, uint16_t dWidth, uint16_t aWidth, int16_t aShift)
: name(tagName), dataWidth(dWidth), addrWidth(aWidth), addrShift(aShift)
{

}
