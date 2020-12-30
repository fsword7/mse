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

class diBus : public DeviceInterface
{
public:
	diBus(device_t *owner);
	~diBus();

	void registerMemory(mapMemoryBlock *block, offs_t base, offs_t size, int prio);

	uint8_t  read8(offs_t addr, ProcessorDevice *cpu = nullptr);
	uint16_t read16(offs_t addr, ProcessorDevice *cpu = nullptr);
	uint32_t read32(offs_t addr, ProcessorDevice *cpu = nullptr);
	uint64_t read64(offs_t addr, ProcessorDevice *cpu = nullptr);

private:
	vector<const mapAddressConfig *> mapConfig;
	vector<mapAddressSpace *> mapSpace;
	vector<mapMemory *> mapMemories;

protected:
	// main memory access
	uint8_t *memData = nullptr;;
	uint64_t memSize = 0;
};
