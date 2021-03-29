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
//
// +--+--+--+--+--+--+--+--+
// |CY|AC|F0|RB| 1|  Stack |
// +--+--+--+--+--+--+--+--+
//  07 06 05 04 03 02 01 00

#define PSW_CY		0x80	// Carry bit
#define PSW_AC		0x40	// Auxiliary carry bit
#define PSW_F0		0x20	// Flag 0 bit
#define PSW_RB		0x10	// Register bank
#define PSW_MBO		0x08	// MBO (must be one) bit
#define PSW_STK		0x07	// Stack pointer

#define STS_IBF		0x02
#define STS_OBF		0x01

// Feature mask bit definitions
#define ARCH_MB			0x01
#define ARCH_EXTBUS		0x02
#define ARCH_UPI41		0x10
#define ARCH_I802X		0x20
#define ARCH_I8048		(ARCH_MB|ARCH_EXTBUS)

#define chargeCycles(cycle)

// internal program access
#define readp(a)		mapProgram->read8(a)
#define writep(a, v)	mapProgram->write8(a, v)

// internal data access
//#define readd(a)		mapData->read(a)
//#define writed(a, v)	mapData->write(a, v)
//#define accessd(a)		mapData->access(a)
#define readd(a)		idata[a]
#define writed(a, v)	idata[a] = v
#define accessd(a)		&idata[a]

// external bus access
#define readb()
#define writeb(v)

// external I/O port access
#define readio(a)
#define writeio(a, v)

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
	void    push_pc_psw();
	void    pull_pc_psw();
	void    pull_pc();
	void    executeJcc(bool flag);
	void opADD(uint8_t data);
	void opADDC(uint8_t data);

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

	uint8_t  tReg = 0;          // Timer register

	uint16_t a11Addr;			// 11-bit address enable    (0x000 or 0x800)
	uint16_t pcAddr;			// Program counter register (0x000 - 0x7FF)

	bool     eaFlag = true;			// External ROM access enable

	bool     xirqEnable = false;    // External interrupt enable
	bool     tirqEnable = false;    // Timer interrupt enable

	bool	 irqFlag = false;
	bool     irqInProgress = false; // IRQ in progress

//	required_shared_ptr<uint8_t> idata;
	uint8_t idata[256]; // internal data memory (temporary - to be removed later)

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
