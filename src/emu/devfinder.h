/*
 * devfinder.h - device/memory finder package
 *
 *  Created on: Oct 19, 2021
 *      Author: Tim Stark
 */

#pragma once

class ObjectFinder
{
public:
    virtual ~ObjectFinder() = default;

    void setObjectName(ctag_t *name);

protected:
    ObjectFinder(Device &base, ctag_t *objName);

    bool validate(bool found, bool required, ctag_t *objName);

    void *findMemoryRegion(uint8_t width, size_t &size, bool required) const;

    Device &base;
    ctag_t *objName;
    bool isResolved = false;
};

template <class ObjectClass, bool Required>
class ObjectFinderCommon : public ObjectFinder
{
public:

    inline ObjectClass *getObject() const { return object; }

    // operator function calls
    inline operator ObjectClass * () const { return object; }
    inline ObjectClass *operator -> () const { assert(object != nullptr); return object; }

protected:
    ObjectFinderCommon(Device &base, ctag_t *objName)
    : ObjectFinder(base, objName)
    { }

    bool validate(ctag_t *objName) { return validate(object != nullptr, Required, objName); }

    ObjectClass *object = nullptr;
};

// template <class ObjectClass, bool Required>
// class ObjectFinderBase : public ObjectFinderCommon<ObjectClass, Required>
// {

// };

// template <class ObjectClass>
// class ObjectFinderBase : public ObjectFinderCommon
// {

// };

template <bool Required>
class MemoryRegionFinder : ObjectFinderCommon<mapMemoryRegion, Required>
{
public:
    MemoryRegionFinder(Device &base, ctag_t *name);

private:
    virtual bool find() override;
};

using RequiredMemoryRegion = MemoryRegionFinder<true>;
using OptionalMemoryRegion = MemoryRegionFinder<false>;

template <typename PointerType, bool Required>
class RegionPointerFinder : public ObjectFinderCommon<PointerType, Required>
{
public:
    RegionPointerFinder(Device &owner, ctag_t *objName)
    : ObjectFinderCommon<PointerType, Required>(owner, objName)
    { }

    inline size_t getLength() const { return size; }
    inline size_t getBytes() const  { return size * sizeof(PointerType); }


private:

    // virtual bool find() override
    // {
    //     assert(!this->isResolved);
    //     this->object = reinterpret_cast<PointerType *>
    //         (this->findMemoryRegion(sizeof(PointerType), size, Required));
    //     this->isResolved = true;

    //     return this->validate("memory region");
    // }

    size_t size = 0;
};

template <typename PointerType> using OptionalRegionPointer = RegionPointerFinder<PointerType, false>;
template <typename PointerType> using RequiredRegionPointer = RegionPointerFinder<PointerType, true>;