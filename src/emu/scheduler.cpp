/*
 *  scheduler.cpp - per-machine device scheduler package
 *
 *  Created on: Sep 22, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/diexec.h"
#include "emu/scheduler.h"

Timer::Timer(Device &dev, int32_t id, void *data, bool flag)
: system(dev.getMachine()),
  device(&dev), ptr(data), param(8),
  enabled(false), temporary(flag),
//   start(system.getLocalTime()),
  period(attotime_t::never),
  expire(attotime_t::never)
{
    callback = TimerDelegate(FUNC(Timer::executeDevice), this);
}

Timer::Timer(Machine &sys, TimerDelegate cb, void *data, bool flag)
: system(&sys), device(nullptr),
  callback(cb), ptr(data), param(8),
  enabled(false), temporary(flag),
//   start(system.getLocalTime()),
  period(attotime_t::never),
  expire(attotime_t::never)
{
}

Timer &Timer::release()
{

    system->getScheduler().removeTimer(this);
    return *this;
}

void Timer::executeDevice(Timer &timer, void *ptr, int64_t param)
{
    timer.device->executeDeviceTimer(timer, ptr, param);
}

bool Timer::enable(bool enable)
{
    const bool oldEnabled = enabled;
    if (oldEnabled != enable)
    {
        enabled = enable;
   
        // Re-schedule that timer.
        DeviceScheduler &scheduler = system->getScheduler();
        scheduler.removeTimer(this);
        scheduler.insertTimer(this);
    }
    return oldEnabled;
}

void Timer::adjust(const attotime_t &duration, int64_t param, const attotime_t &periodic)
{
    DeviceScheduler &scheduler = system->getScheduler();
    if (scheduler.cbTimer == this)
        scheduler.cbTimerModified = true;
    
    this->param = param;
    enabled = true;

    start = scheduler.getCurrentTime();
    expire = start + duration;
    period = periodic;

    // Re-schedule that timer device.
    scheduler.removeTimer(this);
    scheduler.insertTimer(this);

    // If timer device is in head of queue,
    // abort current timeslice.
    if (scheduler.getFirstTimer() == this)
        scheduler.abortTimeslice();
}

void Timer::scheduleNextPeriod()
{
    // Advance for next period
    start = expire;
    expire += period;

    // Re-schedule that timer.
    DeviceScheduler &scheduler = system->getScheduler();
    scheduler.removeTimer(this);
    scheduler.insertTimer(this);
}

// *************************************************************

DeviceScheduler::DeviceScheduler(Machine &system)
: system(system)
{
    // Create a single never-expiring timer device as always one on that timer list.
    // So system will not crash with empty timer list/queue.
    timerList = new Timer(system, TimerDelegate(), nullptr, true);
    timerList->adjust(attotime_t::never);
}

void DeviceScheduler::init()
{
    rebuildExecuteList();
}

attotime_t DeviceScheduler::getCurrentTime() const
{
    if (cbTimer != nullptr)
        return cbTimerExpire;
    // return (execDevice != nullptr) ? execDevice->getLocalTime() : baseTime;
    return baseTime;
}

void DeviceScheduler::addSchedulingQuantum(const attotime_t &quantum, const attotime_t &duration)
{
    assert(quantum.getTime() == 0);

    // attotime_t curTime = baseTime;
    // attotime_t expire = curTime + duration;
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

    // Execute CPU processors with timer device concurrently first
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

    // Now execute timer devices
    executeTimers();
}

void DeviceScheduler::abortTimeslice()
{

}

void DeviceScheduler::executeTimers()
{
    while (timerList->expire <= baseTime)
    {
        Timer &timer = *timerList;
        bool wasEnabled = timer.enabled;
        // Disable timer if period is zero or never.
        if (timer.period.isZero() || timer.period.isNever())
            timer.enabled = false;

        cbTimer = &timer;
        cbTimerExpire = timer.expire;
        cbTimerModified = false;

        // Now executing timer device
        if (wasEnabled)
        {
            if (!timer.callback.isNull())
                timer.callback(timer.ptr, timer.param);
        }

        if (!cbTimerModified)
        {
            if (timer.temporary)
                timer.release();
            else
                timer.scheduleNextPeriod();
        }
    }

    cbTimer = nullptr;
}


Timer *DeviceScheduler::allocateTimer(Device &device, int32_t id, void *data)
{
    Timer *timer = new Timer(device, id, data, false);
    insertTimer(timer);
    return timer;
}

Timer *DeviceScheduler::allocateTimer(TimerDelegate cb, void *data)
{
    Timer *timer = new Timer(system, cb, data, false);
    insertTimer(timer);
    return timer;  
}

void DeviceScheduler::setTimer(Device &device, const attotime_t &duration, int32_t id, void *data, uint64_t param)
{
    Timer *timer = new Timer(device, id, data, false);
    timer->adjust(duration, param);
    insertTimer(timer);
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