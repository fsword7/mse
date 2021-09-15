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
        // Device state flags definition 
        static constexpr int DSF_NOSHOW   = 0x00000001; // No show (invisible)
        static constexpr int DSF_IMPORT   = 0x00000002; // Import function call
        static constexpr int DSF_EXPORT   = 0x00000004; // Export function call
        static constexpr int DSF_SEXT     = 0x00000008; // Signed-extension
        static constexpr int DSF_READONLY = 0x00000020; // Read-only access

        DeviceStateEntry(int index, ctag_t *symbol, int size, uint64_t mask, uint32_t flags, diState *dev);
        ~DeviceStateEntry() = default;

        // Post-construction function call modifiers
        DeviceStateEntry &noshow()               { flags |= DSF_NOSHOW; return *this; };
        DeviceStateEntry &cbImport()             { flags |= DSF_IMPORT; return *this; };
        DeviceStateEntry &cbExport()             { flags |= DSF_EXPORT; return *this; };
        DeviceStateEntry &mask(uint64_t mask)    { dataMask = mask; return *this; }
        DeviceStateEntry &format(string &fmt)    { strFormat = fmt; return *this; }

        inline bool isVisible() const   { return (flags & DSF_NOSHOW) == 0; }
        inline bool isWritable() const  { return (flags & DSF_READONLY) == 0; }
        inline string getSymbol() const { return strSymbol; }
        inline string getFormat() const { return strFormat; }
        inline uint64_t getMask() const { return dataMask; }
        inline void *getPointer() const { return getEntryBase(); }

        uint64_t getValue() const;
        void setValue(uint64_t val) const;

    protected:

        
        // Virtual entry function calls (overrides)
        virtual void *getEntryBase() const { return nullptr; };
        virtual uint64_t getEntryValue() const { return 0; };
        virtual void setEntryValue(uint64_t val) const { };

        diState *device;

        int      index;
        string   strSymbol;
        string   strFormat;
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

        // Override function calls
        void *getEntryBase() const { return &data; }
        uint64_t getEntryValue() const { return data; }
        void setEntryValue(uint64_t val) const { data = val; }

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

        // Override function calls
        void *getEntryBase() const { return &data; }
        uint64_t getEntryValue() const { return data; }
        void setEntryValue(uint64_t val) const { data = bool(val); }

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

    const std::vector<DeviceStateEntry *> &getStateEntries() const { return stateList; }

    void addState(DeviceStateEntry *entry);

private:
    std::vector<DeviceStateEntry *> stateList;
};
