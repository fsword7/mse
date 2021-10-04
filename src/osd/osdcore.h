/*
 *  osdcore.h - OSD interface package
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */

#pragma once

typedef uint64_t osdTicks_t;

osdTicks_t osdGetTicks();
osdTicks_t osdGetTicksPerSecond();
void osdSleep(osdTicks_t ticks);

namespace mse::osd
{
    class osdInteface
    {

    };
}

using osdInterface = mse::osd::osdInteface;