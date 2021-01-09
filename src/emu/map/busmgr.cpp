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

using namespace map;

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

}
