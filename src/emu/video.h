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

        // Timer device callback function calls
        void cbUpdateScreenless(void *data, int64_t param);
        
        // Screenless systems (mainframe, etc)
        Timer *frameTimer = nullptr;

        osdTicks_t avgOversleep = 0;
};