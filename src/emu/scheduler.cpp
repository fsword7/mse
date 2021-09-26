/*
 *  scheduler.cpp - per-machine device scheduler package
 *
 *  Created on: Sep 22, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/diexec.h"
#include "emu/scheduler.h"


Timer &Timer::init(Machine &sys)
{
    system = &sys;

    system->getScheduler().insertTimer(this);
    return *this;
}

Timer &Timer::init(Device &dev)
{
    system = dev.getMachine();
    device = &dev;

    system->getScheduler().insertTimer(this);
    return *this;
}

Timer &Timer::release()
{

    system->getScheduler().removeTimer(this);
    return *this;
}

// *************************************************************

void DeviceScheduler::init()
{
    rebuildExecuteList();
}

void DeviceScheduler::addSchedulingQuantum(const attotime_t &quantum, const attotime_t &duration)
{
    assert(quantum.getTime() == 0);

    // attotime_t curTime = baseTime;
    // attotime_t expire = curTime + duration;

    // QuantumEntry entry;
    // entry.actual = minQuantuam;
    // entry.requested = 
}

void DeviceScheduler::rebuildExecuteList()
{

    if (quantumList.empty())
    {
        // Default 60hz interval scheduling quantum
        attotime_t minQuantuam = attotime_t::fromHz(60);

        addSchedulingQuantum(minQuantuam, attotime_t::never);
    }

    diExecute **activeTail = &execList;
    *activeTail = nullptr;

    for (diExecute &exec : InterfaceIterator<diExecute>(*system.getSystemDevice()))
    {
        // Append to execute list for active devices
        exec.execNext = nullptr;
        *activeTail = &exec;
        activeTail = &exec.execNext;
    }
}

void DeviceScheduler::timeslice()
{

    QuantumEntry *quantum = nullptr;
    for (auto &entry : quantumList)
        if (baseTime < entry.expire)
        {
            quantum = &entry;
            break;
        }

    while (baseTime < timerList->expire)
    {
        attotime_t target(baseTime + attotime_t(quantum->actual));
        if (timerList->expire < target)
            target = timerList->expire;
        

        // Execute all CPU processors concurrently
        for (diExecute *exec = execList; exec != nullptr; exec = exec->execNext)
        {
            attoseconds_t delta = target.getAttoseconds() - exec->localTime.getAttoseconds();
            
            // Can now execute at least one cycle.
            if (delta >= exec->cycleDuration)
            {
                // Set cycle countdown request (cycle per attoseconds)
                exec->cycleRunning = delta / exec->cycleDuration;
                int64_t ranCycles = exec->cycleRunning;

                // if (exec->suspend == 0)
                {
                    // Now executing...
                    *exec->cycleCounter = exec->cycleRunning;
                    exec->cycleStolen = 0;
                    execDevice = exec;
                    exec->executeRun();

                    // Adjust running cycles that we acutally took.
                    assert(ranCycles >= *exec->cycleCounter);
                    ranCycles -= *exec->cycleCounter;
                    assert(ranCycles >= exec->cycleStolen);
                    ranCycles -= exec->cycleStolen;
                }

                // Update total cycles with adjusted running cycles.
                exec->cycleTotal += ranCycles;

                // Update local time in attoseconds
                attotime_t deltaTime = attotime_t(ranCycles * exec->cycleDuration);

                assert(exec->localTime >= attotime_t::zero);
                exec->localTime += deltaTime;
                if (exec->localTime < target)
                    target = (exec->localTime < baseTime) ? baseTime : exec->localTime;
            }
        }

        execDevice = nullptr;
        baseTime = target;
    }

    executeTimers();
}

void DeviceScheduler::executeTimers()
{
}

void DeviceScheduler::insertTimer(Timer *timer)
{
    assert(timer != nullptr);

    const attotime_t expire = timer->enabled ? timer->expire : attotime_t::never;

    // Insert timer in middle of doubly linked timer list
    Timer *ptimer = nullptr;
    for (Timer *ctimer = timerList; ctimer != nullptr; ptimer = ctimer, ctimer = ctimer->next)
    {
        if (ctimer->expire > expire)
        {
            timer->prev = ptimer;
            timer->next = ctimer;

            if (ptimer != nullptr)
                ptimer->next = timer;
            else
                timerList = timer;        
            ctimer->prev = timer;

            return;
        }
    }

    // Insert timer after last one
    if (ptimer != nullptr)
        ptimer->next = timer;
    else
        timerList = timer;
    timer->prev = ptimer;
    timer->next = nullptr;
}

void DeviceScheduler::removeTimer(Timer *timer)
{
    assert(timer != nullptr);

    // Remove itself from previous linked list
    if (timer->prev != nullptr)
        timer->prev = timer->next;
    else
        timerList = timer->next;

    // Remove itself from next linked list
    if (timer->next != nullptr)
        timer->next->prev = timer->prev;
}

void DeviceScheduler::dumpTimers(Console *user)
{

}