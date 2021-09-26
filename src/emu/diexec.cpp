/*
 * diexec.cpp - Device Interface - Execution
 *
 *  Created on: Nov 27, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/diexec.h"

diExecute::diExecute(device_t *owner)
: DeviceInterface(owner, "execute"),
  ownerDevice(*owner)
{
	owner->ifExecute = this;
}

void diExecute::setPCAddress(offs_t addr)
{
}

bool diExecute::load(ifstream &fin, offs_t off)
{
	return false;
}

void diExecute::step(Console *user)
{

}

void diExecute::execute()
{
//	thread myThis;

	run();
}

void diExecute::halt()
{
//	thread myThis;

//	sendSignal(CPU_HALT);
//	myThis.join();
}

void diExecute::ifUpdateClock()
{
	cyclePerSecond = executeClockToCycle(ownerDevice.getClock());
	cycleDuration = HZ_TO_ATTOSECONDS(cyclePerSecond);

	// printf("Clock: %lld Cycle/Second: %lld Time: %lld\n", ownerDevice.getClock(),
	// 	cyclePerSecond, uint64_t(cycleDuration / ATTOSECONDS_PER_NANOSECOND));
}

void diExecute::eatCycles(int64_t cycles)
{
	if (!isExecuting() || cycleCounter == nullptr)
		return;

	if (cycles < 0 || cycles > *cycleCounter)
		*cycleCounter = 0;
	else
		*cycleCounter -= cycles;
}

void diExecute::abortTimeslice()
{
	if (!isExecuting() || cycleCounter == nullptr)
		return;

	int64_t delta = *cycleCounter;

	cycleStolen += delta;
	cycleRunning -= delta;
	*cycleCounter -= delta;
}

void diExecute::runTimeslice()
{

}
