/*
 * device.h - device handling package
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/delegate.h"
#include "emu/list.h"

class SystemConfig;
class Device;

template <class SystemClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
struct system_tag_struct
{
	typedef SystemClass type;
};

template <class SystemClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
auto system_tag_func()
{
	return system_tag_struct<SystemClass, shortName, fullName, fileName>{};
}

template <class SystemClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
constexpr auto systemCreator = &system_tag_func<SystemClass, shortName, fullName, fileName>;


template <class DeviceClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
struct device_tag_struct
{
	typedef DeviceClass type;
};

template <class DeviceClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
auto device_tag_func()
{
	return device_tag_struct<DeviceClass, shortName, fullName, fileName>{};
}

template <class DeviceClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
constexpr auto deviceCreator = &device_tag_func<DeviceClass, shortName, fullName, fileName>;

class DeviceType
{
private:
	typedef Device *(*create_func)(const SystemConfig &config, const DeviceType &type,
			cstag_t &tagName, Device *owner, uint64_t clock);

	template <typename SystemClass>
	static Device *createSystem(const SystemConfig &config, const DeviceType &type,
		cstag_t &tagName, Device *owner, uint64_t clock)
	{
		return new SystemClass(config, type, tagName, clock);
	}

	template <typename DeviceClass>
	static Device *createDevice(const SystemConfig &config, const DeviceType &type,
		cstag_t &tagName, Device *owner, uint64_t clock)
	{
		return new DeviceClass(config, tagName, owner, clock);
	}

public:
	DeviceType() = default;

	template <class SystemClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
	DeviceType(system_tag_struct<SystemClass, shortName, fullName, fileName>(*)())
	: idType(typeid(SystemClass)),
	  shortName(shortName),
	  fullName(fullName),
	  fileName(fileName),
	  creator(&createSystem<SystemClass>)
	{}

	template <class DeviceClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
	DeviceType(device_tag_struct<DeviceClass, shortName, fullName, fileName>(*)())
	: idType(typeid(DeviceClass)),
	  shortName(shortName),
	  fullName(fullName),
	  fileName(fileName),
	  creator(&createDevice<DeviceClass>)
	{}

	Device *create(const SystemConfig &config, cstag_t &tagName, Device *owner, uint64_t clock) const
	{
		assert(creator != nullptr);
		return creator(config, *this, tagName, owner, clock);
	}

	// Getter function calls
	inline const type_info &getType() const  { return idType; }
	inline ctag_t *getFullName() const       { return fullName; }
	inline ctag_t *getShortName() const      { return shortName; }
	inline ctag_t *getSourceName() const     { return fileName; }

private:
	const type_info &idType  = typeid(nullptr_t);	// device type identification
	create_func      creator = nullptr;				// device creator

	ctag_t *shortName = nullptr;	// Short name
	ctag_t *fullName  = nullptr;	// full name/description
	ctag_t *fileName  = nullptr;	// Name of source file
};

//template <class DeviceClass>
//class SystemCreator : public DeviceType
//{
//public:
//	using DeviceType::DeviceType;
//	using DeviceType::create;
//
//};

template <class DeviceClass>
class DeviceCreator : public DeviceType
{
public:
	using DeviceType::DeviceType;
	using DeviceType::create;

	template <typename... Args>
	DeviceClass *operator ()(SystemConfig &config, cstag_t &devName, Args&&... args) const;
};

#define DECLARE_DEVICE_TYPE(Type, Class) \
	extern DeviceCreator<Class> const &Type;

#define DEFINE_DEVICE_TYPE(Type, Class, ShortName, FullName)	\
	struct Class##_device_traits {								\
		static constexpr ctag_t shortName[] = ShortName;		\
		static constexpr ctag_t fullName[]  = FullName;			\
		static constexpr ctag_t fileName[]  = __FILE__;			\
	};															\
	constexpr ctag_t Class##_device_traits::shortName[];		\
	constexpr ctag_t Class##_device_traits::fullName[];			\
	constexpr ctag_t Class##_device_traits::fileName[];			\
	DeviceCreator<Class> const &Type = deviceCreator<Class, (Class##_device_traits::shortName), \
		(Class##_device_traits::fullName), (Class##_device_traits::fileName)>;

// *************************************************************

class DeviceInterface;
class diExternalBus;
class diExecute;
class diDebug;

class Device
: public Delegate,
  public List<Device>
{
public:
	using ifList_t = vector<DeviceInterface *>;

	virtual ~Device() = default;

	// Getter function calls
	inline cstag_t &getDeviceName() const   { return devName; }
	inline ctag_t  *getFullName() const     { return type.getFullName(); }
	inline ctag_t  *getShortName() const    { return type.getShortName(); }
	inline ctag_t  *getSourceName() const   { return type.getSourceName(); }

	inline const SystemConfig &getSystemConfig() const { return sysConfig; }

	inline bool hasInterface(diExternalBus *&iface) { return (iface = ifBus) != nullptr; }
	inline bool hasInterface(diExecute *&iface)     { return (iface = ifExecute) != nullptr; }
	inline bool hasInterface(diDebug *&iface)       { return (iface = ifDebug) != nullptr; }

	void addInterface(DeviceInterface *iface);
	inline ifList_t getInterfaces() { return ifList; }

	inline bool isStarted() const { return flagStarted; }

	// local device function calls
	void configure(SystemConfig &config);
	void start();
	void stop();
	void reset();

	// device-specific virtual device function calls
	//   do nothing by default
	virtual void devConfigure(SystemConfig &config) {}
	virtual void devStart() {}
	virtual void devStop() {}
	virtual void devReset() {}

	void completeConfig(); // complete final configuration

protected:
	Device(const SystemConfig &config, const DeviceType &type, cstag_t &tagName, Device *owner, uint64_t clock);

private:
	const DeviceType   &type;
	const SystemConfig &sysConfig;

	bool flagStarted = false;

	cstag_t devName;

	// device interface section
	ifList_t ifList;

protected:
	friend class diExternalBus;
	friend class diExecute;
	friend class diDebug;

	diExternalBus *ifBus     = nullptr;
	diExecute     *ifExecute = nullptr;
	diDebug       *ifDebug   = nullptr;
};

typedef Device device_t;

// *************************************************************

class DeviceInterface
{
	friend class Device;

public:
	DeviceInterface(device_t *dev, tag_t *name);
	virtual ~DeviceInterface() = default;

	inline device_t *getDevice() const { return owner; }
	inline const tag_t *getName() const { return tagName; }

	// virtual function calls
	virtual void completeConfig() {}

private:
	DeviceInterface *next = nullptr;
	device_t *owner       = nullptr;
	const tag_t *tagName  = nullptr;
};

// *************************************************************

class DeviceIterator
{
public:
	DeviceIterator(Device &dev) : devRoot(dev) {}


private:
	Device &devRoot;
};
