/*
 * vt100.cpp - DEC VT100 terminal
 *
 *  Created on: Aug 19, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/addrmap.h"
#include "emu/map/map.h"
#include "emu/devsys.h"
#include "emu/driver.h"
#include "emu/machine.h"
#include "emu/engine.h"

#include "dev/cpu/i8080/i8080.h"
#include "dev/video/dec/vt100.h"
#include "terminals/dec/vt100.h"
#include "emu/xtal.h"


using namespace aspace;

uint32_t vt100_vtDevice::vt100_updateScreen(ScreenDevice &screen, bitmap16_t &bitmap, const rect_t &clip)
{
    crt->updateVideo(bitmap, clip);
    return 0;
}

uint8_t vt100_vtDevice::readData(offs_t addr)
{
    return ramData[addr];
}

void vt100_vtDevice::vt100(SystemConfig &config)
{

    i8080(config, cpu, "cpu", XTAL(24'883'200) / 9);
    cpu->setAddressMap(AS_PROGRAM, &vt100_vtDevice::vt100_mem);
    cpu->setAddressMap(AS_IOPORT, &vt100_vtDevice::vt100_iomem);

    screen_t *screen = Screen(config, "screen", scrRaster, Color::amber());
    screen->setScreenArea(XTAL(24'073'400) * 2/3,  102*10, 0, 80*10,  262, 0, 25*10);
    screen->setScreenUpdate(FUNC(vt100_vtDevice::vt100_updateScreen));

    VT100_VIDEO(config, crt, "crt", XTAL(24'073'400));
    crt->setScreenName("screen");
    crt->setCharData("chargen");
    crt->getReadRAMDataCallback().set(FUNC(vt100_vtDevice::readData));
}

void vt100_vtDevice::vt100_init()
{

}

// DEC VT100 memory map
//
// 0000 - 1FFF  ROM space (4 x 2K ROM chips)
// 2000 - 3FFF  RAM space (screen and scratch)
// 4000 - 7FFF  (unassigned space)
// 8000 - 9FFF  ROM space program expansion (4 x 2K ROM chips)
// A000 - BFFF  ROM space program expension (1 x 8K ROM chips)
// C000 - FFFF  (unassigned space)

void vt100_vtDevice::vt100_mem(AddressList &map)
{
    map.setUnmapHigh();
    map(0x0000, 0x1FFF).rom().region("vt100fw");
    map(0x2000, 0x3FFF).ram().share("ram");
    map(0x8000, 0x9FFF).rom();
    map(0xA000, 0xBFFF).rom();
}

// DEC VT100 I/O port address map
//
// 00, 01   Intel 8250 PUSART
// 02       Baud rate generator
// 22       Modem buffer
// 42       Flags buffer (R)
// 42       Brightness D/A latch (W)
// 62       NVR latch
// 82       Keyboard UART data
// A2       Video processor DC012
// C2       Video processor DC011
// E2       Graphics port

void vt100_vtDevice::vt100_iomem(AddressList &map)
{
    map.setUnmapHigh();

    map(0x42, 0x42).w(crt, FUNC(vt100video_t::write8_brightness));
    map(0xA2, 0xA2).w(crt, FUNC(vt100video_t::write8_dc012));
    map(0xC2, 0xC2).w(crt, FUNC(vt100video_t::write8_dc011));
}

static const romEntry_t ROM_NAME(vt100)[] =
{      
    ROM_REGION("vt100fw", 0x2000, 0),
    ROM_LOAD("23-061e2-00.e56", 0x0000, 0x0800, 0, nullptr),
    ROM_LOAD("23-032e2-00.e52", 0x0800, 0x0800, 0, nullptr),
    ROM_LOAD("23-033e2-00.e45", 0x1000, 0x0800, 0, nullptr),
    ROM_LOAD("23-034e2-00.e40", 0x1800, 0x0800, 0, nullptr),

    ROM_REGION("chargen", 0x0800, 0),
    ROM_LOAD("23-018e2-00.e4", 0x0000, 0x0800, 0, nullptr),

	ROM_END
};

TERMINAL(vt100,   nullptr, dec, VT100,  vt100_vtDevice, vt100,  vt100_init,  "DEC", "VT100 terminal", SYSTEM_NOT_WORKING)
