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

void ScreenDevice::startDevice()
{

}

void ScreenDevice::stopDevice()
{

}

void ScreenDevice::resetDevice()
{

}

void ScreenDevice::executeTimerDevice(Timer &timer, TimerDeviceID_t id, void *data, int32_t param)
{

}