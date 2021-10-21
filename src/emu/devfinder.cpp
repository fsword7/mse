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
    owner.registerObject(this);
}

void ObjectFinder::setObjectName(ctag_t *name)
{
    assert(!isResolved);

    objName = name;
}

void *ObjectFinder::findMemoryRegion(uint8_t width, size_t &size, bool required) const
{
    mapMemoryRegion *region = base.findMemoryRegion(objName);

    if (region == nullptr)
    {
        size = 0;
        return nullptr;
    }

    // fmt::printf("%s: %s width=%d  region=%d\n", base.getDeviceName(), objName, width, region != nullptr);

    // if (region->getBitWidth() != width)
    // {
    //     // if (required)
    //     size = 0;
    //     return nullptr;
    // }

    // memory region had been found.. all done.
    size = region->getSize() / width;
    return region->getBase();
}

void *ObjectFinder::findMemoryShared(uint8_t width, size_t &size, bool required) const
{
    mapMemoryShare *share = base.findMemoryShare(objName);

    if (share == nullptr)
    {
        size = 0;
        return nullptr;
    }

    // fmt::printf("%s: %s width=%d  shared=%d\n", base.getDeviceName(), objName, width, share != nullptr);
 
    // if (share->getBitWidth() != width)
    // {
    //     // if (required)
    //     size = 0;
    //     return nullptr;
    // }

    // memory region had been found.. all done.
    size = share->getBytes() / width;
    return share->getData();
}

bool ObjectFinder::validate(bool found, bool required, ctag_t *name)
{
    if (required && objName == nullptr)
    {
        fmt::printf("%s: %s (unknown object name) is not defined as required\n",
            base.getDeviceName(), name);
        return false;
    }
    else if (found)
    {
        fmt::printf("%s: %s %s '%s' now found and linked\n",
            base.getDeviceName(), required ? "required" : "optional", name, objName);
        return true;
    }
    else
    {
        if (required)
            fmt::printf("%s: required %s '%s' not found\n", base.getDeviceName(), name, objName);
        else if (objName != nullptr)
            fmt::printf("%s: optional %s '%s' not found\n", base.getDeviceName(), name, objName);
        return !required;
    }
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

    return this->validate("region memory");
}