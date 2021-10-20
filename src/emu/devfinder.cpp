/*
 * devfinder.h - device/memory finder package
 *
 *  Created on: Oct 19, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/devfinder.h"


ObjectFinder::ObjectFinder(Device &owner, ctag_t *name)
: base(owner), objName(name)
{
}

void ObjectFinder::setObjectName(ctag_t *name)
{
    assert(!isResolved);

    objName = name;
}

void *ObjectFinder::findMemoryRegion(uint8_t width, size_t &size, bool required) const
{
    mapMemoryRegion *region = nullptr;

    if (region == nullptr)
    {
        size = 0;
        return nullptr;
    }

    if (region->getBitWidth() != width)
    {
        // if (required)
        size = 0;
        return nullptr;
    }

    // memory region had been found.. all done.
    size = region->getSize() / width;
    return region->getBase();
}

void *ObjectFinder::findMemoryShared(uint8_t width, size_t &size, bool required) const
{
    mapMemoryShare *share = nullptr;

    if (share == nullptr)
    {
        size = 0;
        return nullptr;
    }

    if (share->getBitWidth() != width)
    {
        // if (required)
        size = 0;
        return nullptr;
    }

    // memory region had been found.. all done.
    size = share->getBytes() / width;
    return share->getData();
}

bool ObjectFinder::validate(bool found, bool required, ctag_t *objName)
{
    if (required && objName == nullptr)
        fmt::printf("%s: Object name is not definted as required\n", base.getDeviceName());
    else if (found)
        return true;
    else
    {

    }
    return false;
}

// *********************************************************************************

template <bool Required>
MemoryRegionFinder<Required>::MemoryRegionFinder(Device &owner, ctag_t *objName)
: ObjectFinderCommon<mapMemoryRegion, Required>(owner, objName)
{
}

template <bool Required>
bool MemoryRegionFinder<Required>::find()
{
    assert(!this->isResolved);

    this->object = this->base.findMemoryRegion(this->objName);
    this->isResolved = true;

    return this->validate("memory region");
}