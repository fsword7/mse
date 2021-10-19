/*
 * screen.cpp - screen device package for video controllers
 *
 *  Created on: Sep 13, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/scheduler.h"
#include "emu/screen.h"

DEFINE_DEVICE_TYPE(Screen, ScreenDevice, "Screen", "Video Screen");

ScreenDevice::ScreenDevice(const SystemConfig &config, cstag_t &tagName, Device *owner, uint64_t clock)
: Device(config, Screen, tagName, owner, clock),
  driver(config.getSystemDriver()),
  updateScreen16(*this),
  updateScreen32(*this)
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

    // Re-initialize screen bitmaps
    // reallocateScreenBitmaps();

    // Set timing parameters
    framePeriod = period;
    scanTime = framePeriod / height;
    pixelTime = framePeriod / (height * width);
    vblankPeriod = vblankSupplied ? vblank :
        scanTime * (height - visualArea.getHeight());
    
    // attoseconds_t delta = (getMachine()->getTime() - vblankStartTime).getTime();
    // if (delta < framePeriod)
    //     vblankBeginTimer->adjust(getTimeUntilVBlankStart());
    // else
    //     startVBlank();

    // if (getVPosition() != 0)
    //     scanline0Timer->adjust(getTimeUntilPos(0, 0));
    // else
    //     resetPartialUpdates();
}

void ScreenDevice::startDevice()
{

    // Initialize timer devices
    // vblankBeginTimer = allocateTimer(TID_VBLANK_START);
    // vblankEndTimer = allocateTimer(TID_VBLANK_END);
    // scanline0Timer = allocateTimer(TID_SCANLINE0);
    // scanlineTimer = allocateTimer(TID_SCANLINE);

    // Configure screen with default parameters
    configure(width, height, visualArea, refresh);

    // Set VBLANK start/end time
    vblankStartTime = attotime_t::zero;
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
            break;

        case TID_SCANLINE:
            updateScanline(param);

            param++;
            if (param > visualArea.getBottom())
                param = visualArea.getTop();
            scanlineTimer->adjust(getTimeUntilPos(param, 0), param);
            break;
    }
}

attotime_t ScreenDevice::getTimeUntilPos(int vpos, int hpos) const
{
    assert(vpos >= 0);
    assert(hpos >= 0);

    vpos += height - (visualArea.getBottom() + 1);
    vpos %= height;

    attoseconds_t targetDelta = ((attoseconds_t)vpos * scanTime) + ((attoseconds_t)hpos * pixelTime);
    attoseconds_t currentDelta = (getMachine()->getTime() - vblankStartTime).getTime();
    if (targetDelta <= currentDelta + pixelTime / 2)
        targetDelta += framePeriod;
    while (targetDelta <= currentDelta)
        targetDelta += framePeriod;

    return targetDelta - currentDelta;
}

attotime_t ScreenDevice::getTimeUntilVBlankStart() const
{
    return getTimeUntilPos(visualArea.getBottom() + 1, 0);
}

attotime_t ScreenDevice::getTimeUntilVBlankEnd() const
{
    attotime_t targetTime = vblankEndTime;
    if (!isInVBlanking())
        targetTime += attotime_t(framePeriod);
    return targetTime - getMachine()->getTime();
}

void ScreenDevice::startVBlank()
{
    vblankStartTime = getMachine()->getTime();
    vblankEndTime = vblankStartTime + attotime_t(vblankPeriod);

    // setScreenVBlank(1);

    vblankBeginTimer->adjust(getTimeUntilVBlankStart());
    if (vblankPeriod > 0)
        vblankEndTimer->adjust(getTimeUntilVBlankEnd());
    else
        endVBlank();
}

void ScreenDevice::endVBlank()
{
    // setScreenVBlank(0);
    frameNumber++;
}

// Draw a scanline at once time.
void ScreenDevice::updateScanline(int scanline)
{

}
