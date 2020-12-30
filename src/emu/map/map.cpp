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

mapMemoryBlock::mapMemoryBlock(mapAddressConfig &config, offs_t sAddr, offs_t eAddr, void *base)
: config(config), addrStart(sAddr), addrEnd(eAddr),
  dataBase(reinterpret_cast<uint8_t *>(base))
{
	maxSize  = config.convertAddresstoByte(sAddr + 1 - eAddr);
	dataSize = maxSize;

	// allocate memory space as default
	if (dataBase != nullptr)
	{
		allocated.reserve(dataSize);
		memset(&allocated[0], 0, dataSize);
		dataBase = &allocated[0];
	}
}

void mapMemoryBlock::reserve(offs_t size)
{
	if (size > maxSize)
		size = maxSize;
	allocated.reserve(size);
	memset(&allocated[0], 0, size);
	dataBase = &allocated[0];
	dataSize = size;
}
