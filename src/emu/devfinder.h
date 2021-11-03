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
    
    virtual bool find() = 0;

    inline Device &getOwnerDevice() const { return base; }

protected:
    ObjectFinder(Device &base, ctag_t *objName);

    bool validate(bool found, bool required, ctag_t *objName);

    void *findMemoryRegion(uint8_t width, size_t &size, bool required) const;
    void *findMemoryShared(uint8_t width, size_t &size, bool required) const;

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

    bool validate(ctag_t *objName)
        { return ObjectFinder::validate(object != nullptr, Required, objName); }

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

template <class DeviceClass, bool Required>
class DeviceFinder : public ObjectFinderCommon<DeviceClass, Required>
{
public:
    DeviceFinder(Device &base, ctag_t *name)
    : ObjectFinderCommon<DeviceClass, Required>(base, name)
    { }

    template <typename T>
    std::enable_if_t<std::is_convertible<T *, DeviceClass *>::value, T &> operator = (T &device)
    {
        assert (!this->isResolved);
        // assert (is_expected_tag(device));
        this->object = &device;
        return device;
    }

private:

    virtual bool find() override
    {
        assert(!this->isResolved);
        this->isResolved = true;

        if (Required && this->object != nullptr)
        {
            Device *const device = this->base.findDevice(this->objName);
            this->object = dynamic_cast<DeviceClass *>(device);
            if (device != nullptr && this->object == nullptr)
                fmt::printf("%s: Device '%s' found but is of incorrect type (actual type = %s)\n",
                    this->base.getDeviceName(), device->getDeviceName(), device->getShortName());
        }

        return this->validate("device");
    }
};

template <typename DeviceClass> using RequiredDevice = DeviceFinder<DeviceClass, true>;
template <typename DeviceClass> using OptionalDevice = DeviceFinder<DeviceClass, false>;

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

    inline PointerType & operator [] (int index) { return this->object[index]; }

    inline size_t getLength() const { return size; }
    inline size_t getBytes() const  { return size * sizeof(PointerType); }


private:

    virtual bool find() override
    {
        assert(!this->isResolved);
        this->object = reinterpret_cast<PointerType *>
            (this->findMemoryRegion(sizeof(PointerType), size, Required));
        this->isResolved = true;

        return this->validate("region memory");
    }

    size_t size = 0;
};

template <typename PointerType> using OptionalRegionPointer = RegionPointerFinder<PointerType, false>;
template <typename PointerType> using RequiredRegionPointer = RegionPointerFinder<PointerType, true>;

template <typename PointerType, bool Required>
class SharedPointerFinder : public ObjectFinderCommon<PointerType, Required>
{
public:
    SharedPointerFinder(Device &owner, ctag_t *objName)
    : ObjectFinderCommon<PointerType, Required>(owner, objName)
    { }

    inline PointerType & operator [] (int index) { return this->object[index]; }

    inline size_t getLength() const { return size; }
    inline size_t getBytes() const  { return size * sizeof(PointerType); }


private:

    virtual bool find() override
    {
        assert(!this->isResolved);
        this->object = reinterpret_cast<PointerType *>
            (this->findMemoryShared(sizeof(PointerType), size, Required));
        this->isResolved = true;

        return this->validate("shared memory");
    }

    size_t size = 0;
};

template <typename PointerType> using OptionalSharedPointer = SharedPointerFinder<PointerType, false>;
template <typename PointerType> using RequiredSharedPointer = SharedPointerFinder<PointerType, true>;