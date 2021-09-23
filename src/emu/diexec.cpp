/*
 * diexec.cpp - Device Interface - Execution
 *
 *  Created on: Nov 27, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/diexec.h"

diExecute::diExecute(device_t *owner)
: DeviceInterface(owner, "execute")
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

void diExecute::executeRun()
{
}
