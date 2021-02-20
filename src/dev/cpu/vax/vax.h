/*
 * vax.h - DEC VAX processor emulation package
 *
 *  Created on: Feb 14, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/devproc.h"

// Number of registers
#define CPU_nGREGS		16
#define CPU_nOREGS		20
#define CPU_nPREGS		256
#define CPU_nNREGS		16
#define CPU_nOPCTBL		1024

// Register List
#define REG_nR0			0
#define REG_nR1			1
#define REG_nR2			2
#define REG_nR3			3
#define REG_nR4			4
#define REG_nR5			5
#define REG_nR6			6
#define REG_nR7			7
#define REG_nR8			8
#define REG_nR9			9
#define REG_nR10		10
#define REG_nR11		11
#define REG_nR12		12
#define REG_nR13		13
#define REG_nR14		14
#define REG_nR15		15

#define REG_nAP			REG_nR12 // Argument Pointer
#define REG_nFP			REG_nR13 // Frame Pointer
#define REG_nSP			REG_nR14 // Stack Pointer
#define REG_nPC			REG_nR15 // Program Counter

#define REG_R0			gpReg[REG_nR0].l
#define REG_R1			gpReg[REG_nR1].l
#define REG_R2			gpReg[REG_nR2].l
#define REG_R3			gpReg[REG_nR3].l
#define REG_R4			gpReg[REG_nR4].l
#define REG_R5			gpReg[REG_nR5].l
#define REG_R6			gpReg[REG_nR6].l
#define REG_R7			gpReg[REG_nR7].l
#define REG_R8			gpReg[REG_nR8].l
#define REG_R9			gpReg[REG_nR9].l
#define REG_R10			gpReg[REG_nR10].l
#define REG_R11			gpReg[REG_nR11].l
#define REG_R12			gpReg[REG_nR12].l
#define REG_R13			gpReg[REG_nR13].l
#define REG_R14			gpReg[REG_nR14].l
#define REG_R15			gpReg[REG_nR15].l

#define REG_AP			gpReg[REG_nAP].l
#define REG_FP			gpReg[REG_nFP].l
#define REG_SP			gpReg[REG_nSP].l
#define REG_PC			gpReg[REG_nPC].l

// Register definition
#define CPU_REGUB(rn)	ZXTB(gpReg[rn].b)
#define CPU_REGUW(rn)	ZXTW(gpReg[rn].w)
#define CPU_REGUL(rn)	ZXTL(gpReg[rn].l)
#define CPU_REGSB(rn)	SXTB(gpReg[rn].b)
#define CPU_REGSW(rn)	SXTW(gpReg[rn].w)
#define CPU_REGSL(rn)	SXTL(gpReg[rn].l)

// Processor flags
#define CPU_INIE    0x80000000 // Interrupt/exception in progress

class vax_cpuDevice : public cpuDevice
{
public:
	vax_cpuDevice(const SystemConfig &config, const DeviceType &type,
		const string &tagName, Device *owner, uint64_t clock, int aWidth);
	virtual ~vax_cpuDevice() = default;

protected:
	struct
	{
		scale32_t  gpRegs[CPU_nGREGS]; // General registers
		uint32_t   ipReg[CPU_nPREGS];  // Processor registers
		uint32_t   opReg[CPU_nOREGS];  // Operand registers
		int32_t    rqReg[CPU_nOREGS];  // Recovery registers
		uint32_t   paReg[CPU_nNREGS];  // Parameter registers

		uint32_t   psReg;               // Processor status register
		uint32_t   ccReg;               //    Condition Codes
		uint32_t   accMode;             //    Access Mode

		uint32_t   opCode;              // Current opcode
		uint32_t   opCount;             // Number of operand registers
		uint32_t   rqCount;             // Number of recovery registers
		uint32_t   paCount;             // Number of parameter registers

		uint32_t   flags;               // Processor flags

	} state;

	mapAddressConfig mapProgramConfig;

	mapAddressSpace *mapProgram = nullptr;

};
