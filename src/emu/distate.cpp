/*
 * distate.cpp - Device Interface - State
 *
 *  Created on: Sep 1, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/distate.h"

diState::diState(device_t *owner)
: DeviceInterface(owner, "state")
{
    stateList.clear();
}

diState::~diState()
{
    for (auto *entry : stateList)
        delete entry;
    stateList.clear();
}

void diState::addState(DeviceStateEntry *entry)
{
    stateList.push_back(entry);
}

DeviceStateEntry::DeviceStateEntry(int index, ctag_t *symbol, int size, uint64_t mask, uint32_t flags, diState *dev)
: device(dev), index(index), symbol(symbol), dataSize(size), dataMask(mask), flags(flags)
{
    // Validate data size
    assert(size == 1 || size == 2 || size == 4 || size == 8);

}