/*
 * diexec.h - Device Interface - Execution
 *
 *  Created on: Nov 27, 2020
 *      Author: Tim Stark
 */

#pragma once

enum irqLine
{
	MAX_IRQ_LINES = 64,
	IRQ_LINE0 = 0,
	IRQ_LINE1 = 1,
	IRQ_LINE2 = 2,
	IRQ_LINE3 = 3,
	IRQ_LINE4 = 4,
	IRQ_LINE5 = 5,
	IRQ_LINE6 = 6,
	IRQ_LINE7 = 7,

	IRQ_NMI   = MAX_IRQ_LINES+0,
	IRQ_RESET = MAX_IRQ_LINES+1,
	IRQ_HALT  = MAX_IRQ_LINES+2
};

class diExecute : public DeviceInterface
{
	friend class DeviceScheduler;
	
public:
	diExecute(device_t *owner);
	virtual ~diExecute() = default;

	// execution state
	enum execState
	{
		execStopped = 0,
		execStopping,
		execRunning,
		execIdle,
		execWait
	};

	inline bool isExecuting() { return pState == execRunning; }

	// Abstract function calls
	virtual void run() = 0;
	virtual void step(Console *user); // single step function call

	virtual void setPCAddress(offs_t addr);

	virtual bool load(ifstream &fin, offs_t off);
//	virtual bool save(ofstream &fout) = 0;

//	void boot();
	void execute();
	void halt();

	void eatCycles(int64_t cycles);
	void abortTimeslice();
	void runTimeslice(); // handle time slice period
	
	attoseconds_t getMinQuantum() const { return cycleAttoseconds * executeGetMinCycles(); }

	// Virtual interface function calls
	void ifUpdateClock() override;

protected:
	inline void setCycleCounter(int64_t *counter) { cycleCounter = counter; }

	// debugging hook funtion calls
	inline void debugInstructionHook(offs_t pcAddr)
	{

	}

	// Virtual execute function calls for scheduler
	virtual uint64_t executeClockToCycle(uint64_t clock) const { return clock; }
	virtual uint64_t executeCycleToClock(uint64_t cycle) const { return cycle; }
	virtual int executeGetMinCycles() const { return 1; }
	virtual int executeGetMaxCycles() const { return 1; }
	virtual int executeGetInputLines() const { return 0; }
	virtual void executeRun() {}

protected:
	execState pState = execStopped;

private:
	Device &ownerDevice;

	// Scheduler parameters
	DeviceScheduler *scheduler = nullptr;

	diExecute *nextExec = nullptr;

	uint64_t cyclePerSecond = 0; // cycles per second (clock rate)
	attoseconds_t cycleAttoseconds = 0; // Attoseconds per cycle

	int64_t  cycleRunning = 0;
	int64_t  cycleStolen = 0;
	int64_t *cycleCounter = nullptr; // opcode/cycle counter
};
