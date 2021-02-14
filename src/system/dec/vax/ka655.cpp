/*
 * mv3900.cpp - MicroVAX 3800/3900 (KA655 board) system driver package
 *
 *  Created on: Jul 14, 2020
 *      Author: Tim Stark
 */


#include "emu/core.h"
#include "emu/devsys.h"
#include "emu/driver.h"

#include "system/dec/vax/ka655.h"

// KA655/KA655X Memory Map
//
// 00000000 +-------------------------------------+
//          |            Main Memory              |
//          |- - - - - - - - - - - - - - - - - - -|
//          |         Up to 64 MB Memory          |
// 04000000 +-------------------------------------+
//          |             Reserved                |
// 10000000 +-------------------------------------+
//          |     Secondary Cache Diagnostic      |
// 14000000 +-------------------------------------+
//          |             Reserved                |
// 20000000 +-------------------------------------+
//          :                                     :
//
// 00000000 +-------------------------------------+
//          |           Main Memory               |
//          |- - - - - - - - - - - - - - - - - - -|
//          |        Up to 512 MB Memory          |
// 20000000 +-------------------------------------+
//          |           QBus I/O space            |
// 20002000 +-------------------------------------+
//          |             Reserved                |
// 20040000 +-------------------------------------+
//          |               ROM                   |
// 20080000 +-------------------------------------+
//          |      Local I/O Register Area        |
// 20200000 +-------------------------------------+
//          |             Reserved                |
// 30000000 +-------------------------------------+
//          |          QBus Memory Space          |
// 303FFFFF +-------------------------------------+
//          |             Reserved                |
// 3FFFFFFF +-------------------------------------+
//
// System Memory Map
//
//   0000 0000 - 03FF FFFF  KA650/KA655 Main Memory (Up to 64 MB)
//   1000 0000 - 13FF FFFF  KA650/KA655 Secondary Cache Diagnostic Space
//   0000 0000 - 1FFF FFFF  KA655X Main Memory (Up to 512 MB)
//
//   2000 0000 - 2000 1FFF  Qbus I/O Page
//   2004 0000 - 2005 FFFF  ROM Space, Halt Protected
//   2006 0000 - 2007 FFFF  ROM Space, Halt Unprotected
//   2008 0000 - 201F FFFF  Local Register Space
//   3000 0000 - 303F FFFF  Qbus Memory Space

// Create system routines
void ka655_sysDevice::mv3900(SystemConfig &config)
{

//	cpu = CVAX(config, "cvax", 0);
//	cpu->setAddressMap(AS_PROGRAM, &ka650_device::mv3900_mem);
//	cpu->setDeviceName("cpu");
//
//	cssc = CSSC(config, "cssc", 0);
////	cpu->setSystemSupport(cssc);
//	cmctl = CMCTL(config, "cmctl", 0);
//	cqbic = CQBIC(config, "cqbic", 0);
//
//	cssc->setDeviceName("cssc");
//	cmctl->setDeviceName("cmctl");
//	cqbic->setDeviceName("cqbic");

	cout << "This is MicroVAX 3900 system" << endl;

}

void ka655_sysDevice::mv3900x(SystemConfig &config)
{

//	cpu = CVAX(config, "cvax", 0);
//	cpu->setAddressMap(AS_PROGRAM, &ka650_device::mv3900x_mem);
//	cpu->setDeviceName("cpu");
//
//	cssc = CSSC(config, "cssc", 0);
////	cpu->setSystemSupport(cssc);
//	cmctl = CMCTL(config, "cmctl", 0);
//	cqbic = CQBIC(config, "cqbic", 0);
//
//	cssc->setDeviceName("cssc");
//	cmctl->setDeviceName("cmctl");
//	cqbic->setDeviceName("cqbic");

	cout << "This is MicroVAX 3800x system" << endl;
}

//void ka655_device::mv3900_mem(mapAddress &map)
//{
//	map.setGlobalMask(0x3FFFFFFF);
//	map(0x00000000, 0x03FFFFFF).ram(); // Up to 64MB main memory
////	map(0x10000000, 0x103FFFFF); // Cache registers
////	map(0x20000000, 0x20001FFF).rw("cqbic", FUNC(cqbic_device::read_io), FUNC(cqbic_device::write_io)); // CQBIC Q22-bus I/O device space
//	map(0x20040000, 0x2007FFFF).rom().mirror(0x20000).region("ka655x-srm");
////	map(0x20080000, 0x2008001F).rw("cqbic", FUNC(cqbic_device::read), FUNC(cqbic_device::write)); // CQBIC I/O registers
////	map(0x20080100, 0x200801FF).rw("cmctl", FUNC(cmctl_device::read), FUNC(cmctl_device::write)); // CMCTL registers
////	map(0x20084000, 0x20084007).rw("ka650", FUNC(ka650_device::read), FUNC(ka650_device::write)); // KA650 board registers
////	map(0x20088000, 0x2008FFFF).rw("cqbic", FUNC(cqbic_device::read_map), FUNC(cqbic_device::write_map)); // CQBIC Q22-bus I/O map registers
////	map(0x20140000, 0x201403FF).rw("cssc", FUNC(cssc_device::read), FUNC(cssc_device::write)); // CSSC registers
//	map(0x20140400, 0x201407FF).ram().share("nvram"); // NVRAM memory space
////	map(0x30000000, 0x303FFFFF).rw("cqbic", FUNC(cqbic_device::read_mem), FUNC(cqbic_device::write_mem)); // CQBIC Q22-bus I/O memory space
//}
//
//void ka655_device::mv3900x_mem(mapAddress &map)
//{
//	map.setGlobalMask(0x3FFFFFFF);
//	map(0x00000000, 0x1FFFFFFF).ram(); // Up to 512MB main memory
////	map(0x20000000, 0x20001FFF).rw("cqbic", FUNC(cqbic_device::read_io), FUNC(cqbic_device::write_io)); // CQBIC Q22-bus I/O device space
//	map(0x20040000, 0x2007FFFF).rom().mirror(0x20000).region("ka655x-srm");
////	map(0x20080000, 0x2008001F).rw("cqbic", FUNC(cqbic_device::read), FUNC(cqbic_device::write)); // CQBIC I/O registers
////	map(0x20080100, 0x200801FF).rw("cmctl", FUNC(cmctl_device::read), FUNC(cmctl_device::write)); // CMCTL registers
////	map(0x20084000, 0x20084007).rw("ka650", FUNC(ka650_device::read), FUNC(ka650_device::write)); // KA650 board registers
////	map(0x20088000, 0x2008FFFF).rw("cqbic", FUNC(cqbic_device::read_map), FUNC(cqbic_device::write_map)); // CQBIC Q22-bus I/O map registers
////	map(0x20140000, 0x201403FF).rw("cssc", FUNC(cssc_device::read), FUNC(cssc_device::write)); // CSSC registers
//	map(0x20140400, 0x201407FF).ram().share("nvram"); // NVRAM memory space
////	map(0x30000000, 0x303FFFFF).rw("cqbic", FUNC(cqbic_device::read_mem), FUNC(cqbic_device::write_mem)); // CQBIC Q22-bus I/O memory space
//}

// Initialize system routines
void ka655_sysDevice::mv3900_init()
{

}

void ka655_sysDevice::mv3900x_init()
{

}

//static const romEntry_t ROM_NAME(mv3900)[] =
//{
//		ROM_REGION32_LE("ka655x-srm", 0x20000, 0),
//		ROM_LOAD("ka655x.bin", 0x00000, 0x20000, nullptr, 0),
//		ROM_END
//};

// MicroVAX 3x00 series
//COMP(mv3500,  nullptr, vax, KA650, ka650_device, mv3500,  mv3500_init,  "DEC", "MicroVAX 3500")
//COMP(mv3600,  nullptr, vax, KA650, ka650_device, mv3600,  mv3600_init,  "DEC", "MicroVAX 3600")
//COMP(mv3800,  nullptr, vax, KA655, ka655_device, mv3800,  mv3800_init,  "DEC", "MicroVAX 3800")
//COMP(mv3800x, nullptr, vax, KA655, ka655_device, mv3800x, mv3800x_init, "DEC", "MicroVAX 3800x")
COMP(mv3900,  nullptr, vax, KA655, ka655_sysDevice, mv3900,  mv3900_init,  "DEC", "MicroVAX 3900")
COMP(mv3900x, nullptr, vax, KA655, ka655_sysDevice, mv3900x, mv3900x_init, "DEC", "MicroVAX 3900x")

// VAXserver 3x00 series
//COMP(vs3500,  nullptr, vax, KA650, ka650_device, mv3500,  mv3500_init,  "DEC", "VAXserver 3500")
//COMP(vs3600,  nullptr, vax, KA650, ka650_device, mv3600,  mv3600_init,  "DEC", "VAXserver 3600")
//COMP(vs3800,  nullptr, vax, KA655, ka655_device, mv3800,  mv3800_init,  "DEC", "VAXserver 3800")
//COMP(vs3800x, nullptr, vax, KA655, ka655_device, mv3800x, mv3800x_init, "DEC", "VAXserver 3800x")
COMP(vs3900,  nullptr, vax, KA655, ka655_sysDevice, mv3900,  mv3900_init,  "DEC", "VAXserver 3900")
COMP(vs3900x, nullptr, vax, KA655, ka655_sysDevice, mv3900x, mv3900x_init, "DEC", "VAXserver 3900x")
