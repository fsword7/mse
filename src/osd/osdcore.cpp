/*
 *  osdcore.cpp - OSD interface package
 *
 *  Created on: Sep 27, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"


osdTicks_t osdGetTicks()
{
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}

osdTicks_t osdGetTicksPerSecond()
{
    return std::chrono::high_resolution_clock::period::den /
           std::chrono::high_resolution_clock::period::num;
}

void osdSleep(osdTicks_t duration)
{
    std::this_thread::sleep_for(std::chrono::high_resolution_clock::duration(duration));
}