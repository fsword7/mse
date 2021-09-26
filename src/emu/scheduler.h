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

class Timer
{
    friend class DeviceScheduler;

    public:
        Timer() = default;
        ~Timer() = default;

        Timer &init(Machine &system);
        Timer &init(Device &device);
        Timer &release();
        
    private:
        Timer *next = nullptr;
        Timer *prev = nullptr;

        Machine *system = nullptr;
        Device  *device = nullptr;

        bool enabled = false;

        attotime_t period;      // repeating frequency of timer
        attotime_t start;       // time when timer starts
        attotime_t expire;      // time when timer expires
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
    public:
        DeviceScheduler(Machine &system)
        : system(system)
        {

        }

        void init();

        void addSchedulingQuantum(const attotime_t &quantum, const attotime_t &duratrion);
        void rebuildExecuteList();
        void executeTimers();
        void timeslice();

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
};