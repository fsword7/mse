/*
 * screen.cpp - screen device package for video controllers
 *
 *  Created on: Sep 13, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/screen.h"

DEFINE_DEVICE_TYPE(Screen, ScreenDevice, "Screen", "Video Screen");

ScreenDevice::ScreenDevice(const SystemConfig &config, cstag_t &tagName, Device *owner, uint64_t clock)
: Device(config, Screen, tagName, owner, clock),
  driver(config.getSystemDriver())
{

}

void ScreenDevice::configure(int nwidth, int nheight, rect_t area, attoseconds_t period)
{
    // Validate settings first
    assert(nwidth > 0);
    assert(nheight > 0);
    assert(area.getLeft() >= 0);
    assert(area.getTop() >= 0);
    assert(area.getLeft() < area.getRight());
    assert(area.getTop() < area.getBottom());
    assert(area.getRight() < nwidth);
    assert(area.getBottom() < nheight);
    assert(period > 0);

    // Set width, height and visual area
    width = nwidth;
    height = nheight;
    visualArea = area;

    // Set timing parameters
    framePeriod = period;
    scanTime = framePeriod / height;
    pixelTime = framePeriod / (height * width);
    vblankPeriod = vblankSupplied ? vblank :
        scanTime * (height - visualArea.getHeight());
    
}

void ScreenDevice::startDevice()
{

    // Configure screen with default parameters
    configure(width, height, visualArea, refresh);

    // Set VBLANK start/end time
    vblankStarTime = attotime_t::zero;
    vblankEndTime = attotime_t(vblankPeriod);

}

void ScreenDevice::stopDevice()
{

}

void ScreenDevice::resetDevice()
{

}

void ScreenDevice::executeTimerDevice(Timer &timer, TimerDeviceID_t id, void *data, int32_t param)
{
    switch (id)
    {
        case TID_VBLANK_START:
            startVBlank();
            break;

        case TID_VBLANK_END:
            endVBlank();
            break;

        case TID_SCANLINE0:
        case TID_SCANLINE:
            break;
    }
}

void ScreenDevice::startVBlank()
{

}

void ScreenDevice::endVBlank()
{

}