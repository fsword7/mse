/*
 * device.h - device handling package
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/delegate.h"

class Device;

template <class SystemClass, tag_t *shortName, tag_t *fullName, tag_t *fileName>
struct system_tag_struct
{
	typedef SystemClass type;
};

template <class SystemClass, tag_t *shortName, tag_t *fullName, tag_t *fileName>
auto system_tag_func()
{
	return system_tag_struct<SystemClass, shortName, fullName, fileName>{};
}

template <class SystemClass, tag_t *shortName, tag_t *fullName, tag_t *fileName>
constexpr auto systemCreator = &system_tag_func<SystemClass, shortName, fullName, fileName>;


template <class DeviceClass, tag_t *shortName, tag_t *fullName, tag_t *fileName>
struct device_tag_struct
{
	typedef DeviceClass type;
};

template <class DeviceClass, tag_t *shortName, tag_t *fullName, tag_t *fileName>
auto device_tag_func()
{
	return device_tag_struct<DeviceClass, shortName, fullName, fileName>{};
}

template <class DeviceClass, tag_t *shortName, tag_t *fullName, tag_t *fileName>
constexpr auto deviceCreator = &device_tag_func<DeviceClass, shortName, fullName, fileName>;


class DeviceType
{
private:
	typedef Device *(*create_func)(const SystemConfig &config, const DeviceType &type);

	template <typename SystemClass>
	static Device *createSystem(const SystemConfig &config, const DeviceType &type)
	{
		return new SystemClass(config, type);
	}

	template <typename DeviceClass>
	static Device *createDevice(const SystemConfig &config, const DeviceType &type)
	{
		return new DeviceClass(config, type);
	}

public:
	DeviceType() = default;

	template <class SystemClass, tag_t *shortName, tag_t *fullName, tag_t *fileName>
	DeviceType(system_tag_struct<SystemClass, shortName, fullName, fileName>(*)())
	: idType(typeid(SystemClass)),
	  shortName(shortName),
	  fullName(fullName),
	  fileName(fileName),
	  creator(&createSystem<SystemClass>)
	{}

	template <class DeviceClass, tag_t *shortName, tag_t *fullName, tag_t *fileName>
	DeviceType(device_tag_struct<DeviceClass, shortName, fullName, fileName>(*)())
	: idType(typeid(DeviceClass)),
	  shortName(shortName),
	  fullName(fullName),
	  fileName(fileName),
	  creator(&createDevice<DeviceClass>)
	{}

	Device *create(const SystemConfig &config)
	{
		assert(creator != nullptr);
		return creator(config, *this);
	}

	// Getter function calls
	inline const type_info &getType() const { return idType; }
	inline tag_t *getFullName() const       { return fullName; }
	inline tag_t *getShortName() const      { return shortName; }
	inline tag_t *getSourceName() const     { return fileName; }

private:
	const type_info &idType  = typeid(nullptr_t);	// device type identification
	create_func      creator = nullptr;				// device creator

	tag_t *shortName = nullptr;	// Short name
	tag_t *fullName  = nullptr;	// full name/description
	tag_t *fileName  = nullptr;	// Name of source file
};

template <class DeviceClass>
class DeviceCreator : public DeviceType
{
public:
	using DeviceType::DeviceType;
	using DeviceType::create;

};


#define DECLARE_DEVICE_TYPE(Type, Class) \
	extern DeviceType<Class> const &Type;

#define DEFINE_DEVICE_TYPE(Type, Class, ShortName, FullName)	\
	struct Class##_device_traits {								\
		static constexpr tag_t *shortName[] = ShortName;		\
		static constexpr tag_t *fullName[]  = FullName;			\
		static constexpr tag_t *fileName[]  = __FILE__;			\
	};															\
	constexpr tag_t Class##_device_traits::shortName[];			\
	constexpr tag_t Class##_device_traits::fullName[];			\
	constexpr tag_t Class##_device_traits::fileName[];			\
	DeviceType<Class> const &Type = deviceCreator<Class, (Class##_device_traits::shortName), \
		(Class##_device_traits::fullName), (Class##_device_traits::fileName)>;

// *************************************************************

class Device : public Delegate
{
public:
	~Device() = default;

	// Getter function calls
	inline tag_t *getFullName() const   { return type.getFullName(); }
	inline tag_t *getShortName() const  { return type.getShortName(); }
	inline tag_t *getSourceName() const { return type.getSourceName(); }

protected:
	Device(const SystemConfig &config, const DeviceType &type);

private:
	const DeviceType &type;

};

typedef Device device_t;
