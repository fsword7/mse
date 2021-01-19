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
// Memory Format with function code
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |    RB   |           Function            |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//
// Operate Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |    RB   | SBZ |0|   Function  |    RC   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |       LIT     |1|   Function  |    RC   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
// Floating-point Operate Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |    RB   |       Function      |    RC   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

// Instruction Flags
// Opcode Modes
#define CLS_GRP    0    // Instructions w/Function Codes
#define CLS_PAL    1    // PALcode Instruction
#define CLS_BRA    2    // Branch Instruction
#define CLS_MEM    3    // Memory Instruction
#define CLS_IOP    4    // Operate Instruction (Integer)
#define CLS_FOP    5    // Floating-Point Instruction

// PALcode Modes
#define PAL_NTYPES 4   // Number of PALcode Types
#define PAL_NCODES 256 // Number of PALcode Functions
#define PAL_RAW    0   // PALcodes for Raw Hardware
#define PAL_VMS    1   // PALcodes for OpenVMS
#define PAL_UNX    2   // PALcodes for Digital UNIX
#define PAL_WNT    3   // PALcodes for Windows NT

// Operand Flags
#define OPR_FUNC   0x80000000  // Function Code
#define OPR_TYPE   0x40000000  // PALcode Type
#define OPR_WNT    0x20000000  //    Windows NT
#define OPR_UNX    0x10000000  //    Digital UNIX
#define OPR_VMS    0x08000000  //    OpenVMS
#define OPR_IPR    0x00040000  // IPR Number
#define OPR_PAL    0x00020000  // PALcode Number
#define OPR_MJP    0x00010000  // Jump Displacement
#define OPR_MDP    0x00008000  // Memory Displacement
#define OPR_BDP    0x00004000  // Branch Displacement
#define OPR_IDX    0x00002000  // Register B - Index
#define OPR_LIT    0x00001000  // 8-bit Literal
#define OPR_RC     0x00000400  // Register C
#define OPR_RB     0x00000200  // Register B
#define OPR_RA     0x00000100  // Register A
#define OPR_NONE   0x00000000  // No Operands (Hardware PALcode)

// Instruction Field Defintions
#define OPP_CODE   26        // Operation Code
#define OPP_JUMP   14        // Jump Code
#define OPP_FUNC    5        // Function Code
#define OPP_LIT    13        // Literal Immediate
#define OPP_RA     21        // Register A
#define OPP_RB     16        // Register B
#define OPP_RC      0        // Register C
#define OPC_LIT   (1u << 12) // Literate Bit
#define OPC_MASK  0x3F       // Opcode Mask
#define FNC_MASK  0x7FF      // Function Mask

#define OP_GETOP(opc)  (((opc) >> OPP_CODE) & OPC_MASK)
#define OP_GETJMP(opc) (((opc) >> OPP_JUMP) & 0x3)
#define OP_GETFNC(opc) (((opc) >> OPP_FUNC) & FNC_MASK)
#define OP_GETRA(opc)  (((opc) >> OPP_RA) & REG_MASK)
#define OP_GETRB(opc)  (((opc) >> OPP_RB) & REG_MASK)
#define OP_GETRC(opc)  (((opc) >> OPP_RC) & REG_MASK)
#define OP_GETLIT(opc) (((opc) >> OPP_LIT) & 0xFF)
#define OP_GETPAL(opc) ((opc) & 0x03FFFFFF)
#define OP_GETBDP(opc) SXT21((opc) & 0x001FFFFF)
#define OP_GETMDP(opc) int16_t(opc) // SXTW((opc)
#define OP_GETMEM(opc) ((opc) & 0xFFFF)
#define OP_GETMJP(opc) ((opc) & 0x3FFF)

//#define OPP_CODE   26
//#define OPM_CODE   0x3F
//#define OPP_RA     21
//#define OPP_RB     16
//#define OPP_RC     0
//#define OPP_MDSP   0
//#define OPM_MDSP   0xFFFF
//#define OPP_BDSP   0
//#define OPM_BDSP   0x1FFFFF
//#define OPP_LIT8   13
//#define OPM_LIT8   0xFF
//#define OPM_LIT    0x1000
//
//#define OPP_IFNC   5
//#define OPM_IFNC   0x3F
//#define OPP_FFNC   5
//#define OPM_FFNC   0x3F
//#define OPP_FRND   11
//#define OPM_FRND   0x3
//#define OPP_PALCD  0
//#define OPM_PALCD  0x3FFFFFF
//
//#define OPP_FTRP   13
//#define OPM_FTRP   0x7
//
//#define OP_GETCD(op)   (((op) >> OPP_CODE) & OPM_CODE)
//#define OP_GETRA(op)   ((op) >> OPP_RA)
//#define OP_GETRB(op)   ((op) >> OPP_RB)
//#define OP_GETRC(op)   ((op) >> OPP_RC)
//#define OP_GETPAL(op)  (((op) >> OPP_PALCD) & OPM_PALCD)
//#define OP_GETIFNC(op) (((op) >> OPP_IFNC) & OPM_IFNC)
//#define OP_GETFFNC(op) (((op) >> OPP_FFNC) & OPM_FFNC)
//#define OP_GETFRND(op) (((op) >> OPP_FRND) & OPM_FRND)
//#define OP_GETLIT8(op) (((op) >> OPP_LIT8) & OPM_LIT8)
//#define OP_GETMDSP(op) (((op) >> OPP_MDSP) & OPM_MDSP)
//#define OP_GETBDSP(op) (((op) >> OPP_BDSP) & OPM_BDSP)
//
//// PALcode Mode
//#define PAL_UNDF	0	// Undefined
//#define PAL_VMS		1	// VMS
//#define PAL_UNIX	2	// UNIX
//#define PAL_NT		3	// Windows NT

// Access Mode
#define ACC_KERNEL		0	// Kernel
#define ACC_EXECUTIVE	1	// Executive
#define ACC_SUPERVISOR	2	// Supervisor
#define ACC_USER		3	// User

// PC field definition
#define PC_PAL_MODE		1	// PC PAL mode field

// Register definitions
#define REG_SIZE	32
#define REG_MASK	(REG_SIZE - 1)
#define REG_ZERO	(REG_SIZE - 1)
#define REG_SP		(REG_SIZE - 2)
#define RREG(reg)	((reg) & REG_MASK)
//#define RREG(reg)	((reg) & REG_MASK) + (state.vpcReg & PC_PAL_MODE) && ((reg) & 0x0c) == 0x04) && state.sde ? (REG_MASK+1) : 0)

// executing instruction definitions
#define RA		OP_GETRA(opWord)
#define RB		OP_GETRB(opWord)
#define RC		OP_GETRC(opWord)
#define RAV		state.iRegs[RA]
#define RBV		state.iRegs[RB]
#define RBVL	((opWord & OPC_LIT) ? OP_GETLIT(opWord) : state.iRegs[RB])
#define RCV		state.iRegs[RC]
#define DISP16	SXTW(opWord)
#define DISP21  SXT21(opWord)

#define SXT21(val) SXTL((int32_t)(((val) & 0x100000) ? ((val) | 0xFFE00000) : ((val) & 0x001FFFFF)))

#define readv8(vAddr)  mapProgram->read8(vAddr, this)
#define readv16(vAddr) mapProgram->read16(vAddr, this)
#define readv32(vAddr) mapProgram->read32(vAddr, this)
#define readv64(vAddr) mapProgram->read64(vAddr, this)

#define writev8(vAddr, data)  mapProgram->write8(vAddr, data, this)
#define writev16(vAddr, data) mapProgram->write16(vAddr, data, this)
#define writev32(vAddr, data) mapProgram->write32(vAddr, data, this)
#define writev64(vAddr, data) mapProgram->write64(vAddr, data, this)

struct opcAlpha
{
	ctag_t   *opName;	// Instruction name
	uint32_t  opFlags;	// Opcode flags
	uint16_t  opCode;	// Opcode code
	uint16_t  fnCode;	// Function code
};

class AlphaProcessor : public ProcessorDevice
{
public:
	AlphaProcessor(const SystemConfig &config, const DeviceType &type,
		const string &tagName, Device *owner, uint64_t clock, int aWidth);
	virtual ~AlphaProcessor() = default;

	void initOpcodeTable();

	void init();
	void execute();

	// Virtual function calls from execution interface
	void step(Console *user) override;
	void setPCAddress(offs_t addr) override;

	void devReset() override { init(); }

	inline void setPC(uint64_t addr) { state.vpcReg = addr; }
	inline void addPC(int32_t disp) { state.vpcReg += disp; }

	inline void nextPC()
	{
		state.vpcReg += 4;
		state.ppcReg += 4;
	}

	mapConfigList getAddressConfigList() const;

	// Debugging tools
	string getStringAddress(offs_t addr);
	int list(Console *cty, offs_t vAddr) override;

	static opcAlpha axp_opCodes[];

	opcAlpha *axpCodes[64];
	opcAlpha *axpCodes10[128];
	opcAlpha *axpCodes11[128];
	opcAlpha *axpCodes12[128];
	opcAlpha *axpCodes13[128];
	opcAlpha *axpCodes14[128];
	opcAlpha *axpCodes15[2048];
	opcAlpha *axpCodes16[2048];
	opcAlpha *axpCodes17[2048];
	opcAlpha *axpCodes18[256];
	opcAlpha *axpCodes1A[4];
	opcAlpha *axpCodes1C[128];

protected:

	// Register definitions (state file package)
	struct stateFile {
		uint64_t iRegs[AXP_NIREGS*2];	// Integer registers  (0-31 - regular, 32-63 - shadow)
		uint64_t fRegs[AXP_NFREGS*2];	// Floating registers (0-31 - regular, 32-63 - shadow)
		uint64_t vpcReg;            	// Virtual program counter register
		uint64_t ppcReg;				// Physical program counter register

		bool sde; // Shadow register enable
	} state;

	mapAddressConfig mapProgramConfig;

	mapAddressSpace *mapProgram = nullptr;
};
