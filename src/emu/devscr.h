/*
 * devscr.h - screen device package for video controllers
 *
 *  Created on: Sep 13, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/map/map.h"

enum scrType
{
    scrInvalid = 0,
    scrRaster,
    scrVector,
    scrPrinter
};

enum scrFormat
{
    scrUndefined = 0,
	scrPalette16,
    scrRGB32,
    scrRGBA32
};

#define SCRFLG_LANDSCAPE  0
#define SCRFLG_PORTRAIT   0

class ScreenDevice : public Device
{
public:
	ScreenDevice(const SystemConfig &config, const DeviceType &type, cstag_t &tagName, uint64_t clock);
	~ScreenDevice() = default;

	// Virtual device function calls
	void devConfigure(SystemConfig &config);

private:
	const SystemDriver &driver;

	scrType type     = scrInvalid;
	scrFormat format = scrUndefined;
	uint64_t flags   = 0;
};

DECLARE_DEVICE_TYPE(SCREEN, ScreenDevice);

using screen_t = ScreenDevice;