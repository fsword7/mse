/*
 * vt100.h - DEC VT100 terminal - DC011/DC012 video controller
 *
 *  Created on: Aug 22, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/divideo.h"

namespace emu::video
{
    class vt100_videoDevice : public Device, public diVideo
    {
        public:
            vt100_videoDevice(const SystemConfig &config, cstag_t &tagName, Device *owner, uint64_t clock);

            // I/O access function calls
            void write8_dc11(offs_t off, uint8_t data);
            void write8_dc12(offs_t off, uint8_t data);
            void write8_brightness(offs_t off, uint8_t data);

        protected:
            vt100_videoDevice(const SystemConfig &config, const DeviceType &type,
			    const string &tagName, Device *owner, uint64_t clock);

            void updateVideo();
            void displayCharacter();
    };
}

DECLARE_DEVICE_TYPE(VT100_VIDEO, emu::video::vt100_videoDevice);