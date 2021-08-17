/*
 * alpha.h - DEC Alpha CPU processor package
 *
 *  Created on: Nov 20, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/devproc.h"
#include "dev/cpu/alpha/axp21264_ipr.h"

#define AXP_NIREGS 32  // 32 general purpose registers
#define AXP_NFREGS 32  // 32 floating registers

// Implementation Version
#define ARCH_EV4   0
#define ARCH_EV5   1
#define ARCH_EV6   2
#define ARCH_EV7   3

// Architectural Feature Bits
#define ARCH_PFM   0x1000
#define ARCH_PRC   0x0200
#define ARCH_MVI   0x0100 // Multimedia Extension
#define ARCH_CIX   0x0004 // Count Extension
#define ARCH_FIX   0x0002 // SQR/FP Convert Extension
#define ARCH_BWX   0x0001 // Byte/Word Extensions

// Exceptions
//#define EXC_RSVIN   0x01  // Reserved Instruction
//#define EXC_RSVOPR  0x02  // Reserved Operand
//#define EXC_ALIGN   0x03  // Operand Alignment
//#define EXC_FPDIS   0x04  // Floating-Point Disable
//#define EXC_TBM     0x08  // Translation Buffer Miss
//#define EXC_FOX     0x10  // Fault on Read/Write/Execute
//#define EXC_ACV     0x14  // Access Control Violation
//#define EXC_TNV     0x18  // Translation Not Valid
//#define EXC_BVA     0x1C  // Bad Virtual Address
//#define EXC_EXECUTE 0x00  // Offset for Execute
//#define EXC_READ    0x01  // Offset for Read
//#define EXC_WRITE   0x02  // Offset for Write

// Internal exception codes for all processors
#define EXC_OPCODE    1    // Illegal opcode
#define EXC_ITBMISS   2    // Istream TB miss
#define EXC_DTBMISS1  3    // Dstream TB miss single
#define EXC_DTBMISS2  4    // Dstream TB miss double w/VPTE
#define EXC_IACV      5    // Istream access violation
#define EXC_FPDIS     6    // Floating-point attempted operation

// Traps
#define TRAP_SWC     0x01 // Software Completion
#define TRAP_INV     0x02 // Invalid Operand
#define TRAP_DZE     0x04 // Divide By Zero
#define TRAP_OVF     0x08 // Overflow
#define TRAP_UNF     0x10 // Underflow
#define TRAP_INE     0x20 // Inexact
#define TRAP_IOV     0x40 // Integer Overflow
#define TRAP_SUMM_RW 0x7F // Trap Summary Mask

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
#define OPR_LIT    0x00001000  // Register B - 8-bit Literal
#define OPR_RC     0x00000400  // Register C
#define OPR_RB     0x00000200  // Register B
#define OPR_RA     0x00000100  // Register A
#define OPR_NONE   0x00000000  // No Operands (Hardware PALcode)
#define OPR_RBL    (OPR_RB|OPR_LIT)

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
#define OP_GETMDP(opc) ((opc) & 0xFFFF)
#define OP_GETMEM(opc) ((opc) & 0xFFFF)
#define OP_GETMJP(opc) ((opc) & 0x3FFF)

// Access Mode
#define ACC_KERNEL		0	// Kernel
#define ACC_EXECUTIVE	1	// Executive
#define ACC_SUPERVISOR	2	// Supervisor
#define ACC_USER		3	// User

// Read/write access flags
#define ACC_READ		0	// read access
#define ACC_WRITE		1	// write access
#define ACC_EXEC		2	// execute access

#define ACC_MODE		0x0003	 // Access mode mask
#define ACC_ALTCM		0x0004   // Alternate current mode flag
#define ACC_VPTE        0x0008   // Virtual page table entry

#define ABORT(why)

#define FPSTART if (state.fpen == 0) ABORT(EXC_FPDIS);

// PC field definition
#define PC_PAL_MODE		1	// PC PAL mode field

// Register definitions
#define REG_SIZE	32
#define REG_MASK	(REG_SIZE - 1)
#define REG_ZERO	(REG_SIZE - 1)
#define REG_SP		(REG_SIZE - 2)
#define RREG(reg)	((reg) & REG_MASK)
//#define RREG(reg)	((reg) & REG_MASK) + (state.vpcReg & PC_PAL_MODE) && ((reg) & 0x0c) == 0x04) && state.sde ? (REG_MASK+1) : 0)

#define RREG2(reg) (((reg) & REG_MASK) + \
	(((state.pcAddr & 1) && (((reg) & 0x0C) == 0x04) && state.sde) ? (REG_MASK+1) : 0))

// executing instruction definitions
#define RA		RREG2(OP_GETRA(state.opWord))
#define RB		RREG2(OP_GETRB(state.opWord))
#define RC		RREG2(OP_GETRC(state.opWord))
#define RAV		state.iRegs[RA]
#define RBV		state.iRegs[RB]
#define RBVL	((state.opWord & OPC_LIT) ? OP_GETLIT(state.opWord) : state.iRegs[RB])
#define RCV		state.iRegs[RC]

#define FA		OP_GETRA(state.opWord)
#define FB		OP_GETRB(state.opWord)
#define FC		OP_GETRC(state.opWord)
#define FAV		state.fRegs[RA]
#define FBV		state.fRegs[RB]
#define FCV		state.fRegs[RC]

#define DISP12  SXT12(state.opWord)
#define DISP16	OP_GETMDP(state.opWord)
#define DISP21  SXT21(state.opWord)

#define SXT12(val) SXTL((int32_t)(((val) & 0x800) ? ((val) | 0xFFFFF000) : ((val) & 0x00000FFF)))
#define SXT21(val) SXTL((int32_t)(((val) & 0x100000) ? ((val) | 0xFFE00000) : ((val) & 0x001FFFFF)))

#define SXT48(val) (((val) & 0x0000800000000000LL) \
	? ((val) | 0xFFFF000000000000LL) : ((val) & 0x0000FFFFFFFFFFFFLL))

//#define readv8(vAddr)  mapProgram->read8(vAddr, this)
//#define readv16(vAddr) mapProgram->read16(vAddr, this)
//#define readv32(vAddr) mapProgram->read32(vAddr, this)
//#define readv64(vAddr) mapProgram->read64(vAddr, this)
//
//#define writev8(vAddr, data)  mapProgram->write8(vAddr, data, this)
//#define writev16(vAddr, data) mapProgram->write16(vAddr, data, this)
//#define writev32(vAddr, data) mapProgram->write32(vAddr, data, this)
//#define writev64(vAddr, data) mapProgram->write64(vAddr, data, this)

#define readv8(vAddr)  readv(vAddr, LN_BYTE)
#define readv16(vAddr) readv(vAddr, LN_WORD)
#define readv32(vAddr) readv(vAddr, LN_LONG)
#define readv64(vAddr) readv(vAddr, LN_QUAD)

#define writev8(vAddr, data)  writev(vAddr, data, LN_BYTE)
#define writev16(vAddr, data) writev(vAddr, data, LN_WORD)
#define writev32(vAddr, data) writev(vAddr, data, LN_LONG)
#define writev64(vAddr, data) writev(vAddr, data, LN_QUAD)

#define MSKQ_BYTE 0x00000000000000FFULL
#define MSKQ_WORD 0x000000000000FFFFULL
#define MSKQ_LONG 0x00000000FFFFFFFFULL
#define MSKQ_QUAD 0xFFFFFFFFFFFFFFFFULL

// Instruction cache definitions
#define ICACHE_ENTRIES		1024
#define ICACHE_LINE_SIZE	512
#define ICACHE_MATCH_MASK	(1ull - (ICACHE_LINE_SIZE * 4))
#define ICACHE_INDEX_MASK	(ICACHE_LINE_SIZE - 1ull)
#define ICACHE_BYTE_MASK	(ICACHE_INDEX_MASK << 2)

#define TB_ENTRIES			16

struct opcAlpha
{
	ctag_t   *opName;	// Instruction name
	uint32_t  opFlags;	// Opcode flags
	uint16_t  opCode;	// Opcode code
	uint16_t  fnCode;	// Function code
};

class alpha_cpuDevice : public cpuDevice
{
public:
	alpha_cpuDevice(const SystemConfig &config, const DeviceType &type,
		const string &tagName, Device *owner, uint64_t clock, int aWidth);
	virtual ~alpha_cpuDevice() = default;

	void initOpcodeTable();

	void init();
	void execute();
	void run();

	// Virtual function calls from execution interface
	void step(Console *user) override;
	void setPCAddress(offs_t addr) override;
	bool load(ifstream &fin, offs_t off) override;

	void devReset() override { init(); }


	inline void setPALAddress(offs_t addr) { state.palBase = addr; }

	mapConfigList getAddressConfigList() const;

	// Debugging tools
	string getStringAddress(offs_t addr);
	int list(Console *cty, offs_t vAddr) override;

	static opcAlpha axp_opCodes[];

protected:
	// Architecture function calls
	void setArchType(int type)        { ahType = type; }
	void setArchFlags(uint16_t flags) { ahFlags = flags; }

	int getArchType() const           { return ahType; }
	uint16_t getArchFlags() const     { return ahFlags; }


	int fetchi(uint64_t vAddr, uint32_t &opc);

	// Virtual address translation function calls
	int findTBEntry(uint64_t vAddr, uint32_t accFlags);
	void addTBEntry(uint64_t vAddr, uint64_t pteAddr, uint64_t pteFlags, int accFlags);
	void addITBEntry(uint64_t vAddr, uint64_t pteAddr);
	void addDTBEntry(uint64_t vAddr, uint64_t pteAddr);
	uint64_t getVAForm(uint64_t vAddr, bool type);

	void tbia(int acc);
	void tbiap(int acc);
	void tbis(uint64_t addr, int acc);

	uint64_t checkv(uint64_t vAddr, uint32_t flags, bool &asmb, int &status);
	uint64_t readp(uint64_t pAddr, int size);
	uint64_t readv(uint64_t vAddr, int size);
	void     writep(uint64_t pAddr, uint64_t data, int size);
	void     writev(uint64_t vAddr, uint64_t data, int size);

	virtual void preset() = 0;
	virtual int  translate(uint64_t vAddr, uint64_t &pAddr, bool &asmb, int accFlags) = 0;
	virtual void enterException(int excCode) = 0;

	// Virtual PAL hardware instruction function calls
	virtual void call_pal(uint32_t opWord) = 0; // PAL00 instruction
	virtual void hw_mfpr(uint32_t opWord) = 0;  // PAL19 instruction
	virtual void hw_mtpr(uint32_t opWord) = 0;  // PAL1D instruction
	virtual void hw_ld(uint32_t opWord) = 0;    // PAL1B instruction
	virtual void hw_st(uint32_t opWord) = 0;    // PAL1F instruction
	virtual void hw_ret(uint32_t opWord) = 0;   // PAL1E instruction

	// inline function calls - program counter address register
	inline void setPC(uint64_t addr) { state.pcAddr = addr; }
	inline void addPC(int32_t disp)  { state.pcAddr += disp; }
	inline void nextPC()             { state.pcAddr += 4; }


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

private:
	int      ahType = 0;
	uint16_t ahFlags = 0;

protected:

	bool     bIntrFlag = false;

	// Register definitions (state file package)
	struct stateFile {
		uint64_t iRegs[AXP_NIREGS*2];	// Integer registers  (0-31 - regular, 32-63 - shadow)
		uint64_t fRegs[AXP_NFREGS];	    // Floating registers (0-31 - regular)

		uint64_t pcAddr;				// Program counter address (virtual)
		uint64_t fpcAddr;				// Faulting program counter address (current)

		uint64_t palBase;				// PAL base address
		uint64_t excAddr;				// Exception - PC Address
		uint64_t excMask;               // Exception - Destination of registers

		uint32_t opWord;                // Current Instruction Register

		bool     hwe;                   // Hardware instruction enable
		bool     sde;                   // Shadow register enable
		int      fpen;					// Floating-point enable
		uint64_t fpcr;                  // Floating-point control register

		int      cc_ena;                // CC enable
		uint32_t ccOffset;              // CC offset
		uint32_t cc;                    // CC counter

		int      icm;                   // current access mode
		int      dcm;					// DTB access mode
		int      altcm;					// Alternative access mode

		int      ispe, mspe;	// Super page enable
		int      asn;			// Address Space Number


		int      ppcen;
		int      asten;			// AST interrupt enable
		int      sien;			// Software interrupt enable
		int      pcen;			// Performance counter interrupt enable
		int      cren;			// Corrected data interrupt enable
		int      slen;			// Serial line interrupt enable
		int      eien;			// External interrupt enable

		bool     irq;
		int      intid;			// Interrupt ID
		int      iplr;			// Interrupt Priority Level

		uint32_t eir;           // External interrupt request
		uint32_t sir;			// Software interrupt request
		uint32_t astrr;			// AST interrupt request
		uint32_t aster;			// AST interrupt enable
		uint32_t pcr;			// Performance counter interrupt request
		uint32_t crr;			// Corrected data interrupt request
		uint32_t slr;			// Serial line interrupt request
		uint32_t sum;			// Summary interrupt

		int       asn0;			// Address space number
		int       asn1;			// Address space number
		int       iva_mode;		// ITB Virtual Address Mode
		int       dva_mode;		// DTB Virtual Address Mode

		uint64_t  tbvAddr;		// TB virtual address pending
		uint64_t  ivptb;		// ITB Virtual Process Table Base
		uint64_t  dvptb;		// DTB Virtual Process Table Base
		uint64_t  fvAddr;		// Faulting virtual address
		uint64_t  mmstat;		// Memory Management Status

		uint64_t  pctr;			// Performance counter (not implemented yet)
		int       istat;
		int       dcstat;

		// Onchip instruction cache
		bool iCacheEnable; // Instruction Cache Enable
		int  iCacheLast;   // Last ICACHE entry (current)
		int  iCacheNext;   // Next ICACHE entry to use
		struct ICache
		{
			bool     valid;						// Valid bit
			bool     asmb;						// Address Space Match bit
			int      asn;						// Address Space Number
			uint64_t vAddr;						// Virtual address
			uint64_t pAddr;						// Physical address
			uint32_t data[ICACHE_LINE_SIZE];	// Instruction data
		} iCache[ICACHE_ENTRIES];

		// Translation buffer
		int tbLast[2][2];
		int tbNext[2];
		struct tbEntry
		{
			bool     valid;
			bool     asmb;
			int      asn;
			int      access[2][4];
			int      fault[3];
			uint64_t matchMask;
			uint64_t keepMask;
			uint64_t vAddr;
			uint64_t pAddr;
		} tb[2][TB_ENTRIES];

	} state;

	mapAddressConfig mapProgramConfig;

	mapAddressSpace *mapProgram = nullptr;

	aspace::MemoryAccess<40, 3, 0, LittleEndian>::specific mapProgram1;
	
	Console *log = nullptr;
};

//using AlphaProcessor = alpha_cpuDevice;
