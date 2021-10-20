/*
 *  screen.h - screen device package for video controllers
 *
 *  Created on: Sep 13, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "lib/util/bitmap.h"
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

//#define SCRFLG_LANDSCAPE  0
//#define SCRFLG_PORTRAIT   0

constexpr uint64_t SCRFLAG_LANDSCAPE = 0;
constexpr uint64_t SCRFLAG_PORTRAIT = 0;

class ScreenDevice;

// typedef Delegate<void (ScreenDevice &)> vblankUpdateDelegate;
typedef DeviceDelegate<uint32_t (ScreenDevice &, bitmap16_t &, const rect_t &)> screenUpdateDelgate16;
typedef DeviceDelegate<uint32_t (ScreenDevice &, bitmap32_t &, const rect_t &)> screenUpdateDelgate32;

class ScreenDevice : public Device
{
public:

	// Timer Device ID definitions
	enum {
		TID_VBLANK_START,
		TID_VBLANK_END,
		TID_SCANLINE0,
		TID_SCANLINE
	};

	ScreenDevice(const SystemConfig &config, cstag_t &tagName, Device *owner, uint64_t clock);
	
	ScreenDevice(const SystemConfig &config, cstag_t &tagName, Device *owner, scrType type)
	: ScreenDevice(config, tagName, owner, 0)
	{ 
		setScreenType(type);
	}

	ScreenDevice(const SystemConfig &config, cstag_t &tagName, Device *owner, scrType type, Color color)
	: ScreenDevice(config, tagName, owner, 0)
	{ 
		setScreenType(type);
		setColor(color);
	}

	~ScreenDevice() = default;

	// Post-constructor setting function calls

	// Configure screen parameter settings for visible display
	ScreenDevice &setScreenArea(uint64_t pixClock,
		uint16_t htotal, uint16_t hstart, uint16_t hend,
		uint16_t vtotal, uint16_t vstart, uint16_t vend)
	{
		assert(pixClock != 0);

		// Set visual coordinates with video blank edges
		width = htotal;
		height = vtotal;
		visualArea.set(hstart, hend ? hend-1 : htotal-1, vstart, vend-1);

		// Calculate timing settings
		// clock = pixColor;
		refresh = HZ_TO_ATTOSECONDS(pixClock) * htotal * vtotal;
		vblank = refresh / vtotal * (vtotal - (vend - vstart));

		return *this;
	}

	ScreenDevice &setSize(uint16_t w, uint16_t h)
	{
		width = w;
		height = h;
		return *this;
	}

	ScreenDevice &setVisualArea(int xmin, int xmax, int ymin, int ymax)
	{ 
		visualArea.set(xmin, xmax, ymin, ymax);
		return *this;
	}

	ScreenDevice &setVisualAreaFull()
	{
		 visualArea.set(0, width-1, 0, height-1);
		 return *this;
	}

	ScreenDevice &setColor(Color newColor)
	{
		color = newColor;
		return *this;
	}

	ScreenDevice &setVBlankTime(attoseconds_t time)
	{
		vblank = time;
		vblankSupplied = true;
		return *this;
	}

	template <typename F> std::enable_if_t<screenUpdateDelgate16::supportCallback<F>::value>
	setScreenUpdate(F &&callback, ctag_t *name)
	{
		updateScreen16.set(std::forward<F>(callback), name);
		updateScreen32 = screenUpdateDelgate32(*this);
	}

	template <typename F> std::enable_if_t<screenUpdateDelgate32::supportCallback<F>::value>
	setScreenUpdate(F &&callback, ctag_t *name)
	{
		updateScreen32.set(std::forward<F>(callback), name);
		updateScreen16 = screenUpdateDelgate16(*this);
	}

	// Virtual device function calls
	void startDevice() override;
	void stopDevice() override;
	void resetDevice() override;
	void executeTimerDevice(Timer &timer, TimerDeviceID_t id, void *data, int32_t param) override;

	inline scrType getScreenType() const { return type; }
	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
	inline rect_t getVisualArea() const { return visualArea; }

	inline double getXOffset() const { return xOffset; }
	inline double getYOffset() const { return yOffset; }
	inline double getXScale() const { return xScale; }
	inline double getYScale() const { return yScale; }

	inline attoseconds_t getRefreshRate() const { return refresh; }
	inline attoseconds_t getVBlankRate() const { return vblank; }

	inline void setRefresh(attoseconds_t time) { vblank = time; }
	inline void setScreenType(scrType sType) { type = sType; }

	void setDefaultPosition(double xoffs, double yoffs, double xscale, double yscale)
	{
		xOffset = xoffs;
		yOffset = yoffs;
		xScale = xscale;
		yScale = yscale;
	}

	void configure(int width, int height, rect_t area, attoseconds_t framePeriod);

	// Timer device function calls
	void startVBlank();
	void endVBlank();

	void updateScanline(int scanline);

	attotime_t getTimeUntilVBlankStart() const;
	attotime_t getTimeUntilVBlankEnd() const;
	attotime_t getTimeUntilPos(int vpos, int hpos) const;

	inline bool isInVBlanking() const { return getMachine()->getTime() < vblankEndTime; }
	inline bool isInHBlanking() const { return false; }

private:
	const SystemDriver &driver;

	scrType type     = scrInvalid;
	scrFormat format = scrUndefined;
	uint64_t flags   = 0;
	uint64_t frameNumber = 0;

	double xOffset = 0.0;
	double yOffset = 0.0;
	double xScale = 1.0;
	double yScale = 1.0;

	// Screen parameter definitions
	int width = 100;
	int height = 100;
	rect_t visualArea = { 0, width-1, 0, height-1 };

	uint8_t brightness = 0xFF;
	Color color = Color::white();

	// Timing parameter definitions
	bool          vblankSupplied = false;
	attoseconds_t refresh = 0;
	attoseconds_t vblank = 0;

	attoseconds_t framePeriod = 0;
	attoseconds_t scanTime = 0;
	attoseconds_t pixelTime = 0;
	attoseconds_t vblankPeriod = 0;
	attotime_t    vblankStartTime = attotime_t::zero;
	attotime_t    vblankEndTime = attotime_t::zero;

	Timer *vblankBeginTimer = nullptr;
	Timer *vblankEndTimer = nullptr;
	Timer *scanline0Timer = nullptr;
	Timer *scanlineTimer = nullptr;

	screenUpdateDelgate16 updateScreen16;
	screenUpdateDelgate32 updateScreen32;
};
 
DECLARE_DEVICE_TYPE(Screen, ScreenDevice);

using screen_t = ScreenDevice;