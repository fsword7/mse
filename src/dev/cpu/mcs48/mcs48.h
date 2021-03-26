/*
 * mcs48.h - MCS 48 processor emulation package
 *
 *  Created on: Feb 14, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/devproc.h"

class mcs48_cpuDevice : public cpuDevice
{
public:
	mcs48_cpuDevice(const SystemConfig &config, const DeviceType &type,
		const string &tagName, Device *owner, uint64_t clock, int aWidth);
	virtual ~mcs48_cpuDevice() = default;

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


	mapAddressConfig mapProgramConfig;
	mapAddressConfig mapDataConfig;

	mapAddressSpace *mapProgram = nullptr;
	mapAddressSpace *mapData = nullptr;
};


class mcs8035_cpuDevice : public mcs48_cpuDevice
{
public:
	mcs8035_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~mcs8035_cpuDevice() = default;

};

class mcs8039_cpuDevice : public mcs48_cpuDevice
{
public:
	mcs8039_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~mcs8039_cpuDevice() = default;

};

class mcs8040_cpuDevice : public mcs48_cpuDevice
{
public:
	mcs8040_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~mcs8040_cpuDevice() = default;

};

class mcs8048_cpuDevice : public mcs48_cpuDevice
{
public:
	mcs8048_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~mcs8048_cpuDevice() = default;

};

class mcs8049_cpuDevice : public mcs48_cpuDevice
{
public:
	mcs8049_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~mcs8049_cpuDevice() = default;

};

class mcs8050_cpuDevice : public mcs48_cpuDevice
{
public:
	mcs8050_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~mcs8050_cpuDevice() = default;

};

class mcs8749_cpuDevice : public mcs48_cpuDevice
{
public:
	mcs8749_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~mcs8749_cpuDevice() = default;

};

class mcs8750_cpuDevice : public mcs48_cpuDevice
{
public:
	mcs8750_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~mcs8750_cpuDevice() = default;

};

DECLARE_DEVICE_TYPE(mcs8035, mcs8035_cpuDevice);
DECLARE_DEVICE_TYPE(mcs8039, mcs8039_cpuDevice);
DECLARE_DEVICE_TYPE(mcs8040, mcs8040_cpuDevice);
DECLARE_DEVICE_TYPE(mcs8049, mcs8049_cpuDevice);
DECLARE_DEVICE_TYPE(mcs8050, mcs8050_cpuDevice);
DECLARE_DEVICE_TYPE(mcs8749, mcs8749_cpuDevice);
DECLARE_DEVICE_TYPE(mcs8750, mcs8750_cpuDevice);
