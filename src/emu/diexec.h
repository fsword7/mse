/*
 * diexec.h - Device Interface - Execution
 *
 *  Created on: Nov 27, 2020
 *      Author: Tim Stark
 */

#pragma once

class diExecute : public DeviceInterface
{
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

	// Abstract function calls
	virtual void run() = 0;
	virtual void step(Console *user) = 0; // single step function call

	virtual void setPCAddress(offs_t addr) = 0;

	virtual bool load(ifstream &fin) = 0;
//	virtual bool save(ofstream &fout) = 0;

//	void boot();
	void runx();
	void halt();

protected:
	execState pState = execStopped;
};
