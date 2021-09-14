/*
 * divideo.cpp - Device Interface - Video
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/divideo.h"

diVideo::diVideo(device_t *owner)
: DeviceInterface(owner, "video")
{
}