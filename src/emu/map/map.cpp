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

// **********************************************************************

mapMemoryBlock::mapMemoryBlock(mapAddressSpace &space, offs_t sAddr, offs_t eAddr, void *base)
: space(space), addrStart(sAddr), addrEnd(eAddr),
  dataBase(reinterpret_cast<uint8_t *>(base))
{
	dataSize = sAddr + 1 - eAddr;

	// allocate memory space as default
	if (dataBase != nullptr)
	{
		allocated.reserve(dataSize);
		memset(&allocated[0], 0, dataSize);
		dataBase = &allocated[0];
	}
}
