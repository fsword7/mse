/*
 * busmgr.cpp - external bus manager package
 *
 *  Created on: Jan 2, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/machine.h"
#include "emu/dibus.h"
#include "emu/map/addrmap.h"
#include "emu/map/map.h"

using namespace aspace;

//struct mapSpaceInfo
//{
//	cstag_t name;
//	mapSpaceType type;
//} asSpaceType[] =
//{
//		{ "program", mapSpaceType::asProgram },
//		{ "data",    mapSpaceType::asData },
//		{ "I/O port", mapSpaceType::asPort }
//};
//
//void BusManager::allocate(Console *cty, diExternalBus &bus)
//{
//	Device *dev = bus.getDevice();
//	int width;
//
//	fmt::printf("%s: Allocating address space (%d space%s)\n",
//		dev->getDeviceName(), bus.getAddressConfigCount(),
//		bus.getAddressConfigCount() != 1 ? "s" : "");
//
//	for (int space = 0; space < bus.getAddressConfigCount(); space++)
//	{
//		const mapAddressConfig *config = bus.getAddressConfig(asSpaceType[space].type);
//		if (config == nullptr)
//			continue;
//		fmt::printf("%s: Allocating %s address space...\n",
//			dev->getDeviceName(), asSpaceType[space].name);
//
//		switch (config->getEndianType())
//		{
//		case LittleEndian:
//			width = config->getAddrWidth();
//			switch (width) {
//
//			case 8:  bus.allocate<map::AddressSpaceAccess<8,  0, LittleEndian>>(*this, space);  break;
//			case 16: bus.allocate<map::AddressSpaceAccess<16, 0, LittleEndian>>(*this, space);  break;
//			case 32: bus.allocate<map::AddressSpaceAccess<32, 0, LittleEndian>>(*this, space);  break;
//			case 64: bus.allocate<map::AddressSpaceAccess<64, 0, LittleEndian>>(*this, space);  break;
//
//			default:
//				fmt::printf("%s: Invalid address configuration - address %d width %s shift\n",
//					dev->getDeviceName(), config->getAddrWidth(), config->getAddrShift());
//				break;
//			}
//			break;
//
//		case BigEndian:
//		default:
//			fmt::printf("%s: Invalid endian type\n", dev->getDeviceName());
//			break;
//		}
//	}
//}

void BusManager::init(Console *cty)
{
	fmt::printf("%s: External bus initialization\n", system->getDeviceName());
	cout << flush;

	InterfaceIterator<diExternalBus> iter(*system->getSystemDevice());
	vector<diExternalBus *> buses;

	// Find each device that has external bus system
	buses.clear();
	for (diExternalBus &bus : iter) {
		buses.push_back(&bus);
		allocate(cty, bus);
	}

	// Preparing for set up address space access
	for (auto const sbus : buses)
		sbus->prepare(cty);

	// Populating for set up address space access
	for (auto const sbus : buses)
		sbus->populate(cty);

	// Allocating memory space for set up address space access
	for (auto const sbus : buses)
		sbus->allocate(cty);
}

void BusManager::allocateMainMemory(uint64_t size, uint8_t value)
{
	memData = new uint8_t[size];
	memSize = size;
	memset(memData, value, size);
}

uint8_t *BusManager::allocateMemory(AddressSpace *space, offs_t start, offs_t end, offs_t size)
{
	return new uint8_t[size];
}

uint8_t *BusManager::allocateMemory(Device &device, int space, cstag_t &name, int bytes, int width, endian_t type)
{
	// void *data = allocateMemory(device, 0, name, bytes, width, type);

	MemoryBlock *block = new MemoryBlock(name, bytes, width, type);
	blocks.push_back(block);
	return block->getData();
}

MemoryRegion *BusManager::allocateRegion(cstag_t &name, uint64_t size, int width, endian_t type)
{
	MemoryRegion *region = new MemoryRegion(system, name, size, width, type);

	regions.emplace(name, region);
	return region;
}

void BusManager::releaseRegion(cstag_t &name)
{
	regions.erase(name);
}

MemoryRegion *BusManager::findRegion(cstag_t &name)
{
	auto entry = regions.find(name);
	if (entry != regions.end())
		return entry->second;
	return nullptr;
}

MemoryBank *BusManager::findBank(cstag_t &name)
{
	auto entry = banks.find(name);
	if (entry != banks.end())
		return entry->second;
	return nullptr;
}

MemoryShare *BusManager::allocateShare(Device &device, cstag_t &name, size_t bytes, int width, endian_t type)
{
	if (findShare(name) != nullptr)
		return nullptr;

	void *data = allocateMemory(device, 0, name, bytes, width, type);

	MemoryShare *share = new MemoryShare(data, name, width, bytes, type);
	shares.emplace(name, share);
	return share;
}

MemoryShare *BusManager::findShare(cstag_t &name)
{
	auto entry = shares.find(name);
	if (entry != shares.end())
		return entry->second;
	return nullptr;
}