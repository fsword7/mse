/*
 *  video.h - video manager package
 *
 *  Created on: Sep 26, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/video.h"

static constexpr uint32_t DEFAULT_FRAME_RATE = 60;
static attotime_t DEFAULT_FRAME_DURATION = attotime_t::fromHz(DEFAULT_FRAME_RATE);

VideoManager::VideoManager(Machine &sys)
: system(sys), scheduler(system.getScheduler())
{
    // create 60 Hz interval timer device for video manager.
    frameTimer = scheduler.allocateTimer(TimerDelegate(FUNC(VideoManager::cbUpdateScreenless), this));
    frameTimer->adjust(DEFAULT_FRAME_DURATION, 0, DEFAULT_FRAME_DURATION);
}

void VideoManager::cbUpdateScreenless(void *data, int64_t param)
{
    updateFrame();
}

void VideoManager::updateFrame()
{

    attotime_t currentTime = scheduler.getCurrentTime();


    // Now synchronize real time with emulated time
    // Also use CPU throttle control from user.
    synchronizeRealTime(currentTime);
}

// Synchronize real time with emulated time by using throttle control
void VideoManager::synchronizeRealTime(attotime_t emuTime)
{

    while (1)
    {
        // Get clock ticks from host system as real time
    }
}

osdTicks_t VideoManager::sleepUntilTicks(osdTicks_t targetTicks)
{

    osdTicks_t currentTicks = osdGetTicks();
    while (currentTicks < targetTicks)
    {
        osdTicks_t delta = targetTicks - currentTicks;
        if (delta > avgOversleep / 1000)
            delta -= avgOversleep / 1000;
        else
            delta = 0;
        
        bool slept = delta > 0;
        if (slept)
            osdSleep(delta);
        
        osdTicks_t nextTicks = osdGetTicks();

        if (slept)
        {
            osdTicks_t acutalTicks = nextTicks - currentTicks;
            if (acutalTicks > delta)
            {
                osdTicks_t overTicks = 1000 * (acutalTicks - delta);
                avgOversleep = (avgOversleep * 99 + overTicks) / 100;
            }
        }

        currentTicks = nextTicks;
    }

    return currentTicks;
}