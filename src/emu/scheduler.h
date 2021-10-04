/*
 *  scheduler.h - per-machine device scheduler package
 *
 *  Created on: Sep 22, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "lib/util/time.h"

class Machine;
class DeviceScheduler;

typedef NamedDelegate<void (void *, int64_t)> TimerDelegate;

#define TIMER_CALLBACK(name) void name(void *ptr, int64_t param)

class Timer
{
        friend class DeviceScheduler;

    public:
        Timer() = default;
        Timer(Device &device, int32_t id, void *ptr, bool flag);
        Timer(Machine &system, TimerDelegate cb, void *ptr, bool flag);
        ~Timer() = default;

        Timer &release();
        
        static void executeDevice(Timer &timer, void *ptr, int64_t param);

        bool enable(bool enable);
        void adjust(const attotime_t &duration, int64_t param = 0,
            const attotime_t &periodic = attotime_t::never);
        void scheduleNextPeriod();

    private:
        Timer *next = nullptr;
        Timer *prev = nullptr;

        Machine *system = nullptr;
        Device  *device = nullptr;

        bool temporary = false;
        bool enabled = false;

        attotime_t start;       // time when timer starts
        attotime_t expire;      // time when timer expires
        attotime_t period;      // repeating frequency of timer

        // Timer callback function
        TimerDelegate callback;
        void *ptr = nullptr;
        int32_t param = 0;
};

class QuantumEntry
{
    public:
        attoseconds_t actual;
        attoseconds_t requested;
        attotime_t expire;
};

class DeviceScheduler
{
        friend class Timer;

    public:
        DeviceScheduler(Machine &system);

        void init();

        inline Timer *getFirstTimer() const { return timerList; }

        attotime_t getCurrentTime() const;

        void addSchedulingQuantum(const attotime_t &quantum, const attotime_t &duratrion);
        void rebuildExecuteList();
        void executeTimers();
        void timeslice();
        void abortTimeslice();

        Timer *allocateTimer(Device &device, int32_t id, void *data);
        Timer *allocateTimer(TimerDelegate cb, void *data = nullptr);
        void setTimer(Device &device, const attotime_t &duration, int32_t id, void *data, uint64_t param);

        // Timer *allocateTimer(void *data);
        // void setTimer(const attotime_t &duration, void *data, int32_t param);
        void insertTimer(Timer *timer);
        void removeTimer(Timer *timer);
        void dumpTimers(Console *user);

    private:
        Machine &system;
        diExecute *execDevice = nullptr;
        diExecute *execList = nullptr;

        attotime_t baseTime = attotime_t();

        // Periodic interval list
        vector<QuantumEntry> quantumList;
        attoseconds_t minQuantuam = 0;

        // Timer list for each scheduling quantum
        Timer *timerList = nullptr;
        Timer *cbTimer = nullptr;
        attotime_t cbTimerExpire;
        bool cbTimerModified = false;
};