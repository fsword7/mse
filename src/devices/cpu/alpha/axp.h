/*
 * alpha.h - DEC Alpha CPU processor package
 *
 *  Created on: Nov 20, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/devproc.h"

#define AXP_NIREGS 32  // 32 general purpose registers
#define AXP_NFREGS 32  // 32 floating registers

// Alpha Processor Instruction Field
//
// PALcode Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |                     Number                        |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//
// Branch Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |              Displacement               |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//
// Memory Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |    RB   |          Displacement         |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//
// Operate Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |    RB   |       Function      |    RC   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |       LIT     |1|   Function  |    RC   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

#define OPP_CODE   26
#define OPM_CODE   0x3F
#define OPP_RA     21
#define OPP_RB     16
#define OPP_RC     0
#define OPP_MDSP   0
#define OPM_MDSP   0xFFFF
#define OPP_BDSP   0
#define OPM_BDSP   0x1FFFFF
#define OPP_LIT8   13
#define OPM_LIT8   0xFF
#define OPM_LIT    0x1000

#define OPP_IFNC   5
#define OPM_IFNC   0x3F
#define OPP_FFNC   5
#define OPM_FFNC   0x3F
#define OPP_FRND   11
#define OPM_FRND   0x3
#define OPP_PALCD  0
#define OPM_PALCD  0x3FFFFFF

#define OPP_FTRP   13
#define OPM_FTRP   0x7

#define OP_GETCD(op)   (((op) >> OPP_CODE) & OPM_CODE)
#define OP_GETRA(op)   ((op) >> OPP_RA)
#define OP_GETRB(op)   ((op) >> OPP_RB)
#define OP_GETRC(op)   ((op) >> OPP_RC)
#define OP_GETPAL(op)  (((op) >> OPP_PALCD) & OPM_PALCD)
#define OP_GETIFNC(op) (((op) >> OPP_IFNC) & OPM_IFNC)
#define OP_GETFFNC(op) (((op) >> OPP_FFNC) & OPM_FFNC)
#define OP_GETFRND(op) (((op) >> OPP_FRND) & OPM_FRND)
#define OP_GETLIT8(op) (((op) >> OPP_LIT8) & OPM_LIT8)
#define OP_GETMDSP(op) (((op) >> OPP_MDSP) & OPM_MDSP)
#define OP_GETBDSP(op) (((op) >> OPP_BDSP) & OPM_BDSP)

// PALcode Mode
#define PAL_UNDF	0	// Undefined
#define PAL_VMS		1	// VMS
#define PAL_UNIX	2	// UNIX
#define PAL_NT		3	// Windows NT

// Access Mode
#define ACC_KERNEL		0	// Kernel
#define ACC_EXECUTIVE	1	// Executive
#define ACC_SUPERVISOR	2	// Supervisor
#define ACC_USER		3	// User

// PC field definition
#define PC_PAL_MODE		1	// PC PAL mode field

// Register determination definitions
#define REG_MASK	0x1F
#define RREG(reg)	((reg) & REG_MASK)
//#define RREG(reg)	((reg) & REG_MASK) + (state.vpcReg & PC_PAL_MODE) && ((reg) & 0x0c) == 0x04) && state.sde ? (REG_MASK+1) : 0)

// executing instruction definitions
#define RA		RREG(OP_GETRA(inst))
#define RB		RREG(OP_GETRB(inst))
#define RC		RREG(OP_GETRC(inst))
#define RAV		state.iRegs[RA]
#define RBV		state.iRegs[RB]
#define RBVL	((inst & OPM_LIT) ? OP_GETLIT8(inst) : state.iRegs[RB])
#define RCV		state.iRegs[RC]
#define DISP16	SXTW(inst)
#define DISP21	0 //SXT21(inst)


#define READV(vAddr, size, value)
#define READVL(vAddr, size, value)
#define WRITEV(vAddr, size, value)
#define WRITEVC(vAddr, size, value)

class AlphaProcessor : public ProcessorDevice
{
public:
	AlphaProcessor(const SystemConfig &config, const DeviceType &type);
	~AlphaProcessor() = default;

	void init();
	void execute();

	inline void setPC(uint64_t addr) { state.vpcReg = addr; }
	inline void addPC(uint64_t disp) { state.vpcReg += disp; }

	inline void nextPC()
	{
		state.vpcReg += 4;
		state.ppcReg += 4;
	}

protected:

	// Register definitions (state file package)
	struct stateFile {
		uint64_t iRegs[AXP_NIREGS*2];	// Integer registers  (0-31 - regular, 32-63 - shadow)
		uint64_t fRegs[AXP_NFREGS*2];	// Floating registers (0-31 - regular, 32-63 - shadow)
		uint64_t vpcReg;            	// Virtual program counter register
		uint64_t ppcReg;				// Physical program counter register

		bool sde; // Shadow register enable
	} state;

};
