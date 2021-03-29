/*
 * upd7810.h - NEC uPD7810 series emulation package
 *
 *  Created on: Mar 29, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/devproc.h"

#define chargeCycles(cycle)

struct upd7810op_t
{
	cchar_t   *opName;
	cchar_t   *opReg;
	uint8_t    opType;
	uint8_t    opCode;
	uint8_t    opMask;
};

class upd7810_cpuDevice : public cpuDevice
{
protected:
	upd7810_cpuDevice(const SystemConfig &config, const DeviceType &type,
		const string &tagName, Device *owner, uint64_t clock);

public:
	upd7810_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~upd7810_cpuDevice() = default;

	// Virtual function calls from execution interface
	void step(Console *user) override;
	void setPCAddress(offs_t addr) override;
	bool load(ifstream &fin, offs_t off) override;

	void devReset() override { init(); }

	void initOpcodeTable();

	void init();
	void execute();
	void run();

	mapConfigList getAddressConfigList() const;

	// Debugging tools
	string getStringAddress(offs_t addr);
	int list(Console *cty, offs_t vAddr) override;

protected:
	pair16_t pcAddr;
	pair16_t spReg;

	pair16_t eaReg; // Extended accumulator register
	pair16_t bcReg; // B/C register 8/16-bit
	pair16_t deReg; // D/E register 8/16-bit
	pair16_t hlReg; // H/L register 8/16-bit

	mapAddressConfig mapProgramConfig;

	mapAddressSpace *mapProgram = nullptr;

//	static mcs48op_t opTable[];
};


//class i8035_cpuDevice : public mcs48_cpuDevice
//{
//public:
//	i8035_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
//	virtual ~i8035_cpuDevice() = default;
//
//};

DECLARE_DEVICE_TYPE(upd7810, upd7810_cpuDevice);
