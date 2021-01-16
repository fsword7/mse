/*
 * dibus.h - bus interface package
 *
 *  Created on: Dec 16, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/map/map.h"

struct mapMemory
{
	uint64_t  baseAddr = 0;
	uint64_t  endAddr = 0;
	uint64_t  length = 0;
	uint8_t  *data = nullptr;
	int       prio = 0;
};

class diExternalBus : public DeviceInterface
{
private:
	template <typename T, typename U>
	struct is_related_class {
		static constexpr bool value = is_convertible<add_pointer<T>, add_pointer<U>>::value;
	};

	template <typename T, typename U>
	struct is_related_device {
		static constexpr bool value = is_device<T>::value && is_related_class<T, U>::value;
	};

	template <typename T, typename U>
	struct is_related_interface {
		static constexpr bool value = is_interface<T>::value && is_related_class<T, U>::value;
	};

	template <typename T, typename U>
	struct is_unrelated_device {
		static constexpr bool value = is_device<T>::value && !is_related_class<T, U>::value;
	};

	template <typename T, typename U>
	struct is_unrelated_interface {
		static constexpr bool value = is_interface<T>::value && !is_related_class<T, U>::value;
	};


public:
	diExternalBus(device_t *owner, ctag_t *name);
	virtual ~diExternalBus();

//	void registerMemory(mapMemoryBlock *block, offs_t base, offs_t size, int prio);

	virtual mapConfigList getAddressConfigList() const = 0;

	inline int getAddressConfigCount() { return mapConfig.size(); }
	inline int getAddressSpaceCount()  { return mapSpace.size(); }

//	const mapAddressConfig *getAddressConfig(mapSpaceType type) const;
	const mapAddressConfig *getAddressConfig(int space) const;

	aspace::Constructor getAddressMap(int space = 0) const;

	template <typename T, typename U, typename Return, typename... Args>
	enable_if_t<is_unrelated_device<Device, T>::value>
	setAddressMap(int space, T &obj, Return (U::*func)(Args...))
	{
		setAddressMap(space, aspace::Constructor(func, obj.getDeviceName(), &dynamic_cast<U &>(obj)));
	}

//	template <typename T, typename U, typename Return, typename... Args>
//	enable_if_t<is_unrelated_interface<Device, T>::value>
//	setAddressMap(int space, T &obj, Return (U::*func)(Args...))
//	{
//		setAddressMap(space, map::Constructor(func, "", &dynamic_cast<U &>(obj)));
//	}

	template <typename T, typename Return, typename... Args>
	enable_if_t<is_related_class<Device, T>::value>
	setAddressMap(int space, Return (T::*func)(Args...))
	{
		const SystemConfig &config = getDevice()->getSystemConfig();
		Device *dev = config.getConfigDevice();

		fmt::printf("%s: (related device) Set address list map\n", dev->getDeviceName());
		setAddressMap(space, aspace::Constructor(dev->getDeviceName().c_str(), func, &dynamic_cast<T &>(*dev)));
	}

	template <typename T, typename Return, typename... Args>
	enable_if_t<!is_related_class<Device, T>::value>
	setAddressMap(int space, Return (T::*func)(Args...))
	{
		const SystemConfig &config = getDevice()->getSystemConfig();
		Device *dev = config.getConfigDevice();

		fmt::printf("%s: (unrelated device) Set address list map\n", dev->getDeviceName());
		setAddressMap(space, aspace::Constructor(dev->getDeviceName().c_str(), func, &dynamic_cast<T &>(*dev)));
	}

	void setAddressMap(int space, aspace::Constructor map);

	void completeConfig();

	template <typename Space>
	void allocate(mapBusManager &manager, int space)
	{
		assert((space >= 0) && (space < mapConfig.size()));
		assert(mapConfig[space] != nullptr);

		mapSpace.resize(max(int(mapSpace.size()), space+1));
		assert(mapSpace[space] == nullptr);

		mapSpace[space] = new Space(manager, *this, space, mapConfig[space]->getAddrWidth());
	}

	// Address space initialization routines
	void prepare(Console *cty) { for (auto const &space : mapSpace) if (space != nullptr) { space->prepare(cty); }}
	void populate(Console *cty) { for (auto const &space : mapSpace) if (space != nullptr) { space->populate(cty); }}
	void allocate(Console *cty) { for (auto const &space : mapSpace) if (space != nullptr) { space->allocate(cty); }}
//	void locate(Console *cty) { for (auto const &space : mapSpace) if (space != nullptr) { space->locate(cty); }}

private:
	vector<aspace::Constructor> AddressMapList;
	vector<const mapAddressConfig *> mapConfig;
	vector<mapAddressSpace *> mapSpace;
//	vector<mapMemory *> mapMemories;
};
