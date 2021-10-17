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
: system(sys), scheduler(system.getScheduler()),
  logFile(system.getLogFile())
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
void VideoManager::synchronizeRealTime(attotime_t curTime)
{
    // Determine real-time delta ticks from host system
    osdTicks_t deltaTicks = osdGetTicks() - lastTicks;
    lastTicks += deltaTicks;

    // Get ticks per second from host system as real time
    // For Windows system, it uses 1 ns each tick.
    osdTicks_t ticksPerSecond = osdGetTicksPerSecond();
    attoseconds_t perTick = ATTOSECONDS_PER_SECOND / ticksPerSecond * throttleRate;

    if (deltaTicks >= ticksPerSecond)
    {
        logFile->log(LOG_CONSOLE, "Idle: Resync due to real time advancing more than one second.\n");
        return resetTime(curTime);
    }

    attoseconds_t realDelta = deltaTicks * perTick;

    // logFile->log(LOG_CONSOLE, "Idle: emu %lld real %lld ticks (%lld ns)\n",
    //     (curTime - emuTime).getTime() / perTick, deltaTicks, realDelta / ATTOSECONDS_PER_NANOSECOND);

    emuTime = curTime;
    realTime += attotime_t(realDelta);

    attoseconds_t realAhead = (emuTime - realTime).getTime();
    if (realAhead < 0)
        return;

    osdTicks_t targetTicks = lastTicks + realAhead / perTick;

    // logFile->log(LOG_CONSOLE, "Idle: %s seconds (%lld ticks)\n",
    //     (emuTime - realTime).getAsString(), realAhead / perTick);

    // Now idle time until next interval
    deltaTicks = sleepUntilTicks(targetTicks) - lastTicks;
    lastTicks += deltaTicks;
    realTime += attotime_t(deltaTicks * perTick);
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
                // logFile->log(LOG_CONSOLE, "Idle: Overslept for %lld ticks, expected %lld ticks (Avg of overslept %lld)\n",
                //     acutalTicks, delta, avgOversleep);
            }
        }

        currentTicks = nextTicks;
    }

    return currentTicks;
}