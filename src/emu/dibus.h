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
public:
	diExternalBus(device_t *owner, ctag_t *name);
	virtual ~diExternalBus();

//	void registerMemory(mapMemoryBlock *block, offs_t base, offs_t size, int prio);

	virtual mapConfigList getAddressConfigList() const = 0;

	inline int getAddressConfigCount() { return mapConfig.size(); }
	inline int getAddressSpaceCount()  { return mapSpace.size(); }

//	const mapAddressConfig *getAddressConfig(mapSpaceType type) const;
	const mapAddressConfig *getAddressConfig(int space) const;

	void completeConfig();

	template <typename Space>
	void allocate(mapBusManager &manager, int space)
	{
		assert((space >= 0) && (space < mapSpace.size()));
		assert(mapSpace[space] == nullptr);
		assert(mapConfig[space] != nullptr);

		mapSpace.resize(max(int(mapSpace.size()), space+1));
		mapSpace[space] = new Space(manager, *this, space, mapConfig[space]->getAddrWidth());
	}

private:
	vector<const mapAddressConfig *> mapConfig;
	vector<mapAddressSpace *> mapSpace;
//	vector<mapMemory *> mapMemories;
};
