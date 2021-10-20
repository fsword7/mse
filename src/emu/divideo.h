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

	void setScreenName(ctag_t *name) { screenName = name; }
	
protected:
	bool    screenRequired = false;
	Device *screenBase = nullptr;
	ctag_t *screenName = nullptr;

    screen_t *screen = nullptr;
};
