/*
 * distate.h - Device Interface - State
 *
 *  Created on: Sep 1, 2021
 *      Author: Tim Stark
 */

#pragma once

class diState; // class forward

class DeviceStateEntry
{
    public:
        DeviceStateEntry(int index, ctag_t *symbol, int size, uint64_t mask, uint32_t flags, diState *dev);
        ~DeviceStateEntry() = default;

    private:
        diState *device;

        int      index;
        string   symbol;
        int      dataSize;
        uint64_t dataMask;
        uint32_t flags;
};

template <class itemType>
class DeviceStateRegister : public DeviceStateEntry
{
    public:
        DeviceStateRegister(int index, ctag_t *symbol, itemType &data, diState *dev)
        : DeviceStateEntry(index, symbol, sizeof(itemType),
            std::numeric_limits<typename std::make_unsigned<itemType>::type>::max(), 0, dev),
          data(data)
        {
            static_assert(std::is_integral<itemType>().value,
                "Registration of non-integer types not supported");
        }

        void *getPointer() const { return &data; }
        uint64_t getValue() const { return data; }
        void setValue(uint64_t val) const { data = val; }

    private:
        itemType &data;
};

template <>
class DeviceStateRegister<bool> : public DeviceStateEntry
{
    public:
        DeviceStateRegister(int index, ctag_t *symbol, bool &data, diState *dev)
        : DeviceStateEntry(index, symbol, sizeof(bool), 1, 0, dev), data(data)
        {
        }

        void *getPointer() const { return &data; }
        uint64_t getValue() const { return data; }
        void setValue(uint64_t val) const { data = bool(val); }

    private:
        bool &data;
};

class diState : public DeviceInterface
{
public:
	diState(device_t *owner);
	virtual ~diState();

    template <class itemType>
    void addState(int index, ctag_t *symbol, itemType &data)
    {
        assert(symbol != nullptr);
        addState(new DeviceStateRegister<itemType>(index, symbol, data, this));
    }

    void addState(DeviceStateEntry *entry);

private:
    std::vector<DeviceStateEntry *> stateList;
};
