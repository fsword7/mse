/*
 *  scheduler.cpp - per-machine device scheduler package
 *
 *  Created on: Sep 22, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/diexec.h"
#include "emu/scheduler.h"

Timer::Timer(Device &dev, TimerDeviceID_t id, void *data, bool flag)
: system(dev.getMachine()),
  device(&dev), id(id), ptr(data), param(0),
  enabled(false), temporary(flag),
  start(system->getTime()),
  period(attotime_t::never),
  expire(attotime_t::never)
{
    callback = TimerDelegate(FUNC(Timer::executeDevice), this);
}

Timer::Timer(Machine &sys, TimerDelegate cb, void *data, bool flag)
: system(&sys), device(nullptr),
  callback(cb), ptr(data), param(8),
  enabled(false), temporary(flag),
  start(system->getTime()),
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
    timer.device->executeTimerDevice(timer, timer.id, ptr, param);
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
: system(system), logFile(system.getLogFile())
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
    assert(quantum.getTime() != 0);

    attotime_t curTime = baseTime;
    attotime_t expire = curTime + duration;
    
    const attoseconds_t attos = quantum.getAttoseconds();

    vector<QuantumEntry>::iterator insertAfter = quantumList.end();

    for (auto entry = quantumList.begin(); entry != quantumList.end(); entry++)
    {
        // Search for position to being inserted.
        // If quantum scheduler expired, remove it.
        if ((*entry).expire <= expire)
            entry = quantumList.erase(entry);
        else if ((*entry).requested <= attos)
            insertAfter = entry;
    }

    if (insertAfter != quantumList.end() && (*insertAfter).requested == attos)
        (*insertAfter).expire = expire < (*insertAfter).expire ? (*insertAfter).expire : expire;
    else
    {
        QuantumEntry entry;

        entry.actual = attos < minQuantuam ? minQuantuam : attos;
        entry.requested = attos;
        entry.expire = expire;
        quantumList.insert(insertAfter, entry);
    }
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
    // logFile->log(LOG_CONSOLE, "**** Start of quantum scheduler ****\n");

    // Get unexpired quantum scheduler. If quantum
    // scheduler expired, remove it from quantum list.
    assert(!quantumList.empty());
    while ((*quantumList.begin()).expire <= baseTime)
        quantumList.erase(quantumList.begin());
    QuantumEntry &quantum = *quantumList.begin();

    // logFile->log(LOG_CONSOLE, "Quantum interval: %lldns (%llf Hz)\n",
    //     quantum.actual / ATTOSECONDS_PER_NANOSECOND,
    //     ATTOSECCONDS_TO_HZ(quantum.actual));

    // Execute CPU processors with timer device concurrently first
    while (baseTime < timerList->expire)
    {
        attotime_t target(baseTime + attotime_t(quantum.actual));
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

                // logFile->log(LOG_CONSOLE, "%s(%s): Executing for %lld (%lld cycles)\n",
                //     exec->getDevice()->getDeviceName(), exec->getDevice()->getShortName(),
                //     delta, exec->cycleRunning);

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

                // logFile->log(LOG_CONSOLE, "%s(%s): Executed at %lld (%lld total cycles)\n",
                //     exec->getDevice()->getDeviceName(), exec->getDevice()->getShortName(),
                //     ranCycles, exec->cycleTotal);

                if (exec->localTime < target)
                    target = (exec->localTime < baseTime) ? baseTime : exec->localTime;
            }
        }

        execDevice = nullptr;
        baseTime = target;
    }
    // logFile->log(LOG_CONSOLE, "Executed all CPU processors, now executing time devices...\n");
    
    // Now execute timer devices
    executeTimers();

    // logFile->log(LOG_CONSOLE, "**** End of quantum scheduler ****\n");
}

void DeviceScheduler::abortTimeslice()
{

}

void DeviceScheduler::executeTimers()
{
    // logFile->log(LOG_CONSOLE, "Timer: %s  Expire: %s\n",
    //     baseTime.getAsString(), timerList->expire.getAsString());

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
            {
                // if (timer.device != nullptr)
                //     logFile->log(LOG_CONSOLE, "Timer: Executing %s(%s) ID %d\n",
                //         timer.device->getDeviceName(), timer.device->getShortName(), 0);
                // else
                //     logFile->log(LOG_CONSOLE, "Timer: Executing callback %s\n",
                //         timer.callback.getName());
                timer.callback(timer.ptr, timer.param);
            }
        }

        // logFile->log(LOG_CONSOLE, "Timer: Execution done\n");
        
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