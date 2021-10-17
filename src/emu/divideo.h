/*
 * divideo.h - Device Interface - Video
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/screen.h"

class diVideo : public DeviceInterface
{
public:
	diVideo(device_t *owner);
	virtual ~diVideo() = default;

protected:
    screen_t *screen = nullptr;
};
