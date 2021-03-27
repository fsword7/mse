/*
 * mcs48.h - MCS 48 processor emulation package
 *
 *  Created on: Feb 14, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/devproc.h"

#define OPR_NOPE    0x00
#define OPR_REG     0x01
#define OPR_LIT     0x02
#define OPR_ADDR2   0x04
#define OPR_ADDR3   0x08

#define RREG(rn)	gReg[(rn) & 0x7]

// PSW status bit definitions
#define PSW_C		0x80	// Carry bit
#define PSW_A		0x40
#define PSW_F		0x20
#define PSW_B		0x10

#define STS_IBF		0x02
#define STS_OBF		0x01

// Feature mask bit definitions
#define ARCH_MB			0x01
#define ARCH_EXTBUS		0x02
#define ARCH_UPI41		0x10
#define ARCH_I802X		0x20
#define ARCH_I8048		(ARCH_MB|ARCH_EXTBUS)

#define chargeCycles(cycle)

struct mcs48op_t
{
	cchar_t   *opName;
	cchar_t   *opReg;
	uint8_t    opType;
	uint8_t    opCode;
	uint8_t    opMask;
	const int  cycle;
};

class mcs48_cpuDevice : public cpuDevice
{
public:
	mcs48_cpuDevice(const SystemConfig &config, const DeviceType &type,
		const string &tagName, Device *owner, uint64_t clock, int paWidth, int daWidth);
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

	uint8_t fetchi();
	void    updateRegisters();

	void    executeJcc(bool flag);

	// Debugging tools
	string getStringAddress(offs_t addr);
	int list(Console *cty, offs_t vAddr) override;

	// External program space access setup
	void setProgram1K(aspace::AddressList &map);
	void setProgram2K(aspace::AddressList &map);
	void setProgram4K(aspace::AddressList &map);

	// Internal data space access setup
	void setData64(aspace::AddressList &map);
	void setData128(aspace::AddressList &map);
	void setData256(aspace::AddressList &map);

protected:
	uint8_t  opCode;			// Current instruction in progress
	uint8_t *gReg = nullptr;	// registers (pointer to data memory)
	uint8_t  aReg;				// Accumulator register
	uint8_t  pReg[3];			// I/O port registers
	uint8_t  pswReg;			// Processor Status Word register
	bool     f1Flag;			// F1 flag

	uint16_t a11Addr;			// 11-bit address enable    (0x000 or 0x800)
	uint16_t pcAddr;			// Program counter register (0x000 - 0x7FF)

	bool		eaFlag;			// External ROM access enable

	uint8_t *data = nullptr;	// Internal data memory access

	mapAddressConfig mapProgramConfig;
	mapAddressConfig mapDataConfig;
	mapAddressConfig mapPortConfig;

	mapAddressSpace *mapProgram = nullptr;
	mapAddressSpace *mapData = nullptr;
	mapAddressSpace *mapPort = nullptr;

	mcs48op_t *opCodes[256];

	static mcs48op_t opTable[];
};


class i8035_cpuDevice : public mcs48_cpuDevice
{
public:
	i8035_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~i8035_cpuDevice() = default;

};

class i8039_cpuDevice : public mcs48_cpuDevice
{
public:
	i8039_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~i8039_cpuDevice() = default;

};

class i8040_cpuDevice : public mcs48_cpuDevice
{
public:
	i8040_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~i8040_cpuDevice() = default;

};

class i8048_cpuDevice : public mcs48_cpuDevice
{
public:
	i8048_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~i8048_cpuDevice() = default;

};

class i8049_cpuDevice : public mcs48_cpuDevice
{
public:
	i8049_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~i8049_cpuDevice() = default;

};

class i8050_cpuDevice : public mcs48_cpuDevice
{
public:
	i8050_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~i8050_cpuDevice() = default;

};

class i8749_cpuDevice : public mcs48_cpuDevice
{
public:
	i8749_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~i8749_cpuDevice() = default;

};

class i8750_cpuDevice : public mcs48_cpuDevice
{
public:
	i8750_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~i8750_cpuDevice() = default;

};

DECLARE_DEVICE_TYPE(i8035, i8035_cpuDevice);
DECLARE_DEVICE_TYPE(i8039, i8039_cpuDevice);
DECLARE_DEVICE_TYPE(i8040, i8040_cpuDevice);
DECLARE_DEVICE_TYPE(i8049, i8049_cpuDevice);
DECLARE_DEVICE_TYPE(i8050, i8050_cpuDevice);
DECLARE_DEVICE_TYPE(i8749, i8749_cpuDevice);
DECLARE_DEVICE_TYPE(i8750, i8750_cpuDevice);
