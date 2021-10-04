/*
 * vt100.h - DEC VT100 terminal - DC011/DC012 video controller
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/devscr.h"
#include "dev/video/dec/vt100.h"

using namespace emu::video;

DEFINE_DEVICE_TYPE(VT100_VIDEO, vt100_videoDevice, "Vt100_video", "VT100 video controller");

vt100_videoDevice::vt100_videoDevice(const SystemConfig &config, const DeviceType &type,
	const string &tagName, Device *owner, uint64_t clock)
: Device(config, type, tagName, owner, clock),
  diVideo(this)
{

}

vt100_videoDevice::vt100_videoDevice(const SystemConfig &config, cstag_t &tagName, Device *owner, uint64_t clock)
: vt100_videoDevice(config, VT100_VIDEO, tagName, owner, clock)
{

}

void vt100_videoDevice::write8_dc11(offs_t off, uint8_t data)
{

}

void vt100_videoDevice::write8_dc12(offs_t off, uint8_t data)
{

}

void vt100_videoDevice::write8_brightness(offs_t off, uint8_t data)
{

}