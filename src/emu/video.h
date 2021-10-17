/*
 *  video.h - video manager package
 *
 *  Created on: Sep 26, 2021
 *      Author: Tim Stark
 */

#pragma once

class VideoManager
{
    public:
        VideoManager(Machine &system);

        void updateFrame();
        void synchronizeRealTime(attotime_t time);
        osdTicks_t sleepUntilTicks(osdTicks_t ticks);

    private:
        Machine &system;
        DeviceScheduler &scheduler;
        LogFile *logFile = nullptr;

        // Timer device callback function calls
        void cbUpdateScreenless(void *data, int64_t param);

        inline void resetTime(attotime_t time) { emuTime = time; realTime = time; };

        // Screenless systems (mainframe, etc)
        Timer *frameTimer = nullptr;

        double throttleRate = 1.0;
        osdTicks_t avgOversleep = 0;
        osdTicks_t lastTicks = 0;

        attotime_t realTime = attotime_t::zero;
        attotime_t emuTime = attotime_t::zero;
};