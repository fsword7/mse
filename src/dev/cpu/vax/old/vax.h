/*
 * vax.h
 *
 *  Created on: Feb 26, 2017
 *      Author: Timothy Stark
 */

#pragma once

#include "emu/devcpu.h"
#include "emu/debug.h"

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

// Processor Status Register definition
//
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---~
// |CM |TP | 0   0 |FPD|IS |  CUR  |  PRV  | 0 |        IPL        |
// +---+---+---+---^---+---+---+---^---+---+---+---^---+---+---+---~
//  31  30  29  28  27  26  25  24  23  22  21  20  19  18  17  16
//
//     ~---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
//     | 0   0   0   0   0   0   0   0 |DV |FU |IV | T | N | Z | V | C |
//     ~---+---+---+---^---+---+---+---^---+---+---+---^---+---+---+---+
//      15  14  13  12  11  10  09  08  07  06  05  04  03  02  01  00
//

// PSL - Processor Status Longword for Kernel mode
#define PSL_MBZ			 0x3020FF00 // Must Be Zero
#define PSL_CM			 0x80000000 // PDP-11 Compatibility mode
#define PSL_TP			 0x40000000 // Trace Pending
#define PSL_FPD			 0x08000000 // First Part Done
#define PSL_IS			 0x04000000 // Interrupt Stack
#define PSL_CUR			 0x03000000 // Current Mode
#define PSL_PRV			 0x00C00000 // Previous Mode
#define PSL_IPL			 0x001F0000 // Interrupt Priority Level

#define PSL_P_CUR        24   // Position of IPL bits
#define PSL_P_PRV        22   // Position of previous mode bits
#define PSL_P_IPL        16   // Position of current mode bits

#define PSL_M_IPL        0x1F // Interrupt Priority Level Mask
#define PSL_M_ISCUR      0x07 // Access Mode Mask with IS flag
#define PSL_M_MODE       0x03 // Access Mode Mask

#define IPL_MASK		0x1F // Interrupt Priority Level Mask
#define ACC_MASK		0x03 // Access Mode Mask
#define ACI_MASK		0x07 // Access/Interrupt Mode Mask

#define PSL_GETIPL(ps)   (((ps) >> PSL_P_IPL) & IPL_MASK)
#define PSL_GETPRV(ps)   (((ps) >> PSL_P_PRV) & ACC_MASK)
#define PSL_GETCUR(ps)   (((ps) >> PSL_P_CUR) & ACC_MASK)
#define PSL_GETICUR(ps)  (((ps) >> PSL_P_CUR) & ACI_MASK)

#define PSL_SETIPL(ipl)  (((ipl) & IPL_MASK) << PSL_P_IPL)
#define PSL_SETCUR(acc)  (((acc) & ACC_MASK) << PSL_P_CUR)
#define PSL_SETPRV(acc)  (((acc) & ACC_MASK) << PSL_P_PRV)

// PSW - Processor Status Word for User mode
#define PSW_MASK         0xFFFF // PSW User Mask
#define PSW_MBZ          0xFF00 // Must be zeroes
#define PSW_DV			 0x0080 // Decimal Overflow Trap Enable
#define PSW_FU			 0x0040 // Floating Underflow Trap Enable
#define PSW_IV			 0x0020 // Integer Overflow Trap Enable
#define PSW_T			 0x0010 // Trace Enable
#define PSW_CC			 0x000F // Condition Codes

// CC - Condition Codes (part of PSW word)
#define CC_N			 0x0008 // Negative
#define CC_Z			 0x0004 // Zero
#define CC_V			 0x0002 // Overflow
#define CC_C			 0x0001 // Carry
#define CC_MASK			 (CC_N|CC_Z|CC_V|CC_C)

// Access Mode (Protection Codes)
#define AM_KERNEL		 0 // Kernel mode for OS kernel
#define AM_EXECUTIVE	 1 // Executive mode for file system
#define AM_SUPERVISOR	 2 // Supervisor mode for DCL (shell)
#define AM_USER			 3 // User mode for normal programs
#define AM_INTERRUPT     4 // Interrupt mode

// CALL instruction
#define CALL_DV				0x8000
#define CALL_IV				0x4000
#define CALL_MBZ			0x3000
#define CALL_MASK			0x0FFF

// Call stack frame (2nd longword)
#define CALL_SPA			0x03
#define CALL_S				(1u << CALL_P_S)
#define CALL_PSW			0x7FE0

#define CALL_P_SPA			30
#define CALL_P_S			29
#define CALL_P_MASK			16

// PUSHR/POPR instruction
#define STK_MASK			0x7FFF

// MOVC instruction
#define MVC_FWD				0
#define MVC_BACK			1
#define MVC_FILL			3
#define MVC_M_STATE			0x03
#define MVC_P_CC			2

// String instructions
//
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |  Delta PC     |Fill Character |            Length             |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//

#define STR_M_DPC			0xFF   // Delta PC address
#define STR_M_CHR			0xFF   // Fill character
#define STR_M_LEN			0xFFFF // String length
#define STR_P_DPC			24
#define STR_P_CHR			16

#define STR_GETDPC(d)		(((d) >> STR_P_DPC) & STR_M_DPC)
#define STR_GETCHR(d)		(((d) >> STR_P_CHR) & STR_M_CHR)
#define STR_GETLEN(d)		((d) & STR_M_LEN)
#define STR_SETDPC(d)		(((d) & STR_M_DPC) << STR_P_DPC)
#define STR_SETCHR(d)		(((d) & STR_M_CHR) << STR_P_CHR)
#define STR_SETLEN(d)		((d) & STR_M_LEN)
#define STR_PACK(dpc,f,ln)	(STR_SETDPC(dpc) | STR_SETCHR(f) | STR_SETLEN(ln))

// Interrupt/Exception Types
#define IE_INT           0 // Interrupt
#define IE_EXC           1 // Exception
#define IE_SVE           2 // Severe Exception

// System Control Block Vectors
#define SCB_PASSIVE      0x00  // Passive Release
#define SCB_MCHK         0x04  // Machine Check
#define SCB_KSNV         0x08  // Kernel Stack Not Valid
#define SCB_POWER        0x0C  // Power Fail
#define SCB_RESIN        0x10  // Reserved or Privileged Instruction
#define SCB_XFC          0x14  // Customer Reserved Instruction (XFC)
#define SCB_RESOP        0x18  // Reserved Operand
#define SCB_RESAD        0x1C  // Reserved Address Mode
#define SCB_ACV          0x20  // Access-Control Violation
#define SCB_TNV          0x24  // Translation Not Valid
#define SCB_TP           0x28  // Trace Pending
#define SCB_BPT          0x2C  // Breakpoint Instruction
#define SCB_COMPAT       0x30  // Compatibility
#define SCB_ARITH        0x34  // Arithmetic
#define SCB_CHMK         0x40  // Change Mode to Kernel
#define SCB_CHME         0x44  // Change Mode to Executive
#define SCB_CHMS         0x48  // Change Mode to Supervisor
#define SCB_CHMU         0x4C  // Change Mode to User
#define SCB_CRDERR       0x54  // CRD Error Interrupt
#define SCB_MEMERR       0x60  // Memory Error Interrupt
#define SCB_IPLSOFT      0x80  // Software Interrupt Base
#define SCB_SOFTWARE1    0x84  // Software Level 1
#define SCB_SOFTWARE2    0x88  // Software Level 2
#define SCB_SOFTWARE3    0x8C  // Software Level 3
#define SCB_SOFTWARE4    0x90  // Software Level 4
#define SCB_SOFTWARE5    0x94  // Software Level 5
#define SCB_SOFTWARE6    0x98  // Software Level 6
#define SCB_SOFTWARE7    0x9C  // Software Level 7
#define SCB_SOFTWARE8    0xA0  // Software Level 8
#define SCB_SOFTWARE9    0xA4  // Software Level 9
#define SCB_SOFTWARE10   0xA8  // Software Level A
#define SCB_SOFTWARE11   0xAC  // Software Level B
#define SCB_SOFTWARE12   0xB0  // Software Level C
#define SCB_SOFTWARE13   0xB4  // Software Level D
#define SCB_SOFTWARE14   0xB8  // Software Level E
#define SCB_SOFTWARE15   0xBC  // Software Level F
#define SCB_TIMER        0xC0  // Interval Timer
#define SCB_EMULATE      0xC8  // Subset Emulation
#define SCB_EMULFPD      0xCC  // Subset Emulation with FPD flag
#define SCB_CSREAD       0xF0  // Console Storage Read
#define SCB_CSWRITE      0xF4  // Console Storage Write
#define SCB_CTYIN        0xF8  // Console Terminal Read
#define SCB_CTYOUT       0xFC  // COnsole Terminal Write

#define SCB_ADDR         ALIGN_LONG // SCB Address Mask
#define SCB_VECTOR       ALIGN_LONG // SCB Vector Mask
#define SCB_NOPRIV       1
#define SCB_NEXT         1

// Arithmetic Exception Type Codes
#define TRAP_INTOVF   1 // Integer Overflow Trap
#define TRAP_INTDIV   2 // Integer Divide-by-Zero Trap
#define TRAP_FLTOVF   3 // Floating Overflow Trap
#define TRAP_FLTDIV   4 // Floating Divide-by-Zero Trap
#define TRAP_FLDUND   5 // Floating Underflow Trap
#define TRAP_DECOVF   6 // Decimal Overflow Trap
#define TRAP_DECDIV   4 // Decimal Divide-by-Zero Trap
#define TRAP_SUBRNG   7 // Subscript Range Trap

#define FAULT_FLTOVF  8 // Floating Overflow Fault
#define FAULT_FLTDIV  9 // Floating Divide Fault
#define FAULT_FLTUND 10 // Floating Underflow Fault

// SIRR/SISR register definitions
#define SIRR_MASK		 0xF
#define SISR_MASK        0xFFFE
#define SISR_2           (1u << 2)

// ASTLVL register definition
#define AST_MASK         0x07
#define AST_MAX          4

// P0BR/P0LR/P1BR/P1LR register definitions
#define BR_MASK          ALIGN_LONG
#define LR_MASK          0x003FFFFF

// IPR Registers - Validity Checks
#define IPR_PA_TEST(pa) ((pa) & 0xC0000003)
#define IPR_LR_TEST(lr) (ZXTL(lr) > 0x200000)
#define IPR_BR_TEST(br) \
	((((br) & 0xC0000000) != 0x80000000) || ((br) & 0x00000003))

// LDPCTX Instruction - Validity Checks
#define LP_AST_TEST(ast) ((ast) > AST_MAX)
#define LP_P0LR_TEST(lp) ((lp) & 0xF8C00000)
#define LP_P1LR_TEST(lp) ((lp) & 0x7FC00000)

#include "dev/cpu/vax/mmu.h"

// Read/write memory access
#define CACC             0x80000000         // Console memory access
#define RACC             curMode            // Read memory access
#define WACC             (curMode << 4)     // Write memory access
#define CRA              (CACC|RACC)        // Console write memory access
#define CWA              (CACC|WACC)        // Console read memory access
#define AM_MASK(am)      (1u << (am))       // Access mode mask

#define REF_V            0
#define REF_P            1
#define REF_C            2

#define PA_MASK30        0x3FFFFFFF         // 30-bit physical addressing

// Interrupt Priority Level
#define IPL_MAX			IPL_MASK
#define IPL_HALTPIN		IPL_MAX
#define IPL_POWER		0x1E
// KA650 IPL levels
#define IPL_MEMERR		0x1D
#define IPL_CRDERR		0x1A
// Standard IPL levels
#define IPL_HMAX		0x17
#define IPL_HMIN		0x14
#define IPL_SMAX		0x0F

#define IRQ_TRAP        0x0700 // Trap requests
#define IRQ_IPL         0x001F // IPL requests
#define IRQ_M_IPL       0x1F
#define IRQ_M_TRAP      0x07
#define IRQ_P_IPL       0
#define IRQ_P_TRAP      8

#define IRQ_GETIPL(irq)        (((irq) >> IRQ_P_IPL) & IRQ_M_IPL)
#define IRQ_GETTRAP(irq)       (((irq) >> IRQ_P_TRAP) & IRQ_M_TRAP)
#define IRQ_SETTRAP(trap)      ((trap & IRQ_M_TRAP) << IRQ_P_TRAP)

// Processor Interrupt Request Signals
#define IRQ_HALT				0x80000000
#define IRQ_CLOCK				0x00100000
#define IRQ_CRDERR				0x00080000
#define IRQ_MEMERR				0x00040000
#define IRQ_CTYOUT				0x00020000
#define IRQ_CTYIN				0x00010000

#define UpdateIRQ() \
	irqFlags = evaluate();


// Exception Codes - Fault
#define RSVD_INST_FAULT		SCB_RESIN            // Reserved instruction fault
#define RSVD_ADDR_FAULT		SCB_RESAD            // Reserved address fault
#define RSVD_OPND_FAULT		SCB_RESOP            // Reserved operand fault
#define PRIV_INST_FAULT		SCB_RESIN|SCB_NOPRIV // Privileged instruction fault
#define ARITH_FAULT			SCB_ARITH            // Arithmetic Fault
#define HALT_ACTION			SCB_NEXT             // Model-specific halt action - continue

// Exception Codes - Page fault
#define PAGE_TNV            SCB_TNV  // Table Not Valid
#define PAGE_ACV            SCB_ACV  // Access Violation
#define PAGE_KSNV           SCB_KSNV // Kernel Stack Not Valid

// Halt Action Codes
#define HALT_PIN		0x02	// Halt Pin
#define HALT_PWRON		0x03	// Initial Power On
#define HALT_ISNV		0x04	// Interrupt Stack Not Valid During Exception
#define HALT_MCHK		0x05	// Machine Check During Normal Exception
#define HALT_INST		0x06	// HALT Instruction in Kernel Mode
#define HALT_SCB11		0x07	// SCB Vector <1:0> = 11
#define HALT_SCB10		0x08	// SCB Vector <1:0> = 10
#define HALT_CHMFIS		0x0A	// CHMx Executed while on Interrupt Stack
#define HALT_CHMTIS		0x0B	// CHMx Executed to the Interrupt Stack
#define HALT_AMCHK		0x10	// ACV/TNV during Machine Check
#define HALT_AKSNV		0x11	// ACV/TNV during Kernel Stack Not Valid
#define HALT_DMCHK		0x12	// Machine Check during Machine Check
#define HALT_MKSNV		0x13	// Machine Check during Kernel Stack Not Valid
#define HALT_INIE0		0x19	// PSL<26:24> = 101 during Interrupt/Exception
#define HALT_INIE1		0x1A	// PSL<26:24> = 110 during Interrupt/Exception
#define HALT_INIE2		0x1B	// PSL<26:24> = 111 during Interrupt/Exception
#define HALT_REI0		0x1D	// PSL<26:24> = 101 during REI Instruction
#define HALT_REI1		0x1E	// PSL<26:24> = 110 during REI Instruction
#define HALT_REI2		0x1F	// PSL<26:24> = 111 during REI Instruction

// Exception Codes - Stop
#define STOP_HALT			-1		// HALT opcode
#define STOP_UOPC			-2		// Unimplemented opcode
#define STOP_ILLVEC			-3		// Illegal vector
#define STOP_UIPL			-4      // Undefined IPL level
#define STOP_INIE			-5		// Exception during exception
#define STOP_OPR			-6		// Operator request

//enum stopCode {
//	STOP_eHALT = 0,
//	STOP_eUOPC,
//	STOP_eILLVEC,
//	STOP_eUIPL,
//	STOP_eINIE
//};

// Update Condition Codes

// Set Z flag for CLR Instructions
#define SetZ(cc)  cc = (((cc) & CC_C) | CC_Z)

// Set N and Z flag
#define SetNZ(cc, res, src, flg)                 \
	if ((res) < (src))        cc = CC_N | (flg); \
	else if ((res) == (src))  cc = CC_Z | (flg); \
	else                      cc = (flg);

#define SetNZQ(cc, low, high, flg)                   \
	if ((high) < 0)               cc = CC_N | (flg); \
	else if (((low)|(high)) == 0) cc = CC_Z | (flg); \
	else                          cc = (flg);

#define pSetNZO(cc, src, flg)                          \
	if (SXTL(src[3]) < 0)       cc = CC_N | (flg);     \
	else if ((src[0] | src[1] | src[2] | src[3]) == 0) \
		cc = CC_Z | (flg);                             \
	else    cc = (flg);

// Set C flag (Unsigned)
#define SetC(cc, res, src) \
	if ((res) < (src)) cc |= CC_C;

// Set V flag
#define SetV(cc)                              \
	(cc) |= CC_V;                             \
	if (psReg & PSW_IV) {                     \
		iraFlags &= ~IRQ_TRAP;                \
		irqFlags |= IRQ_SETTRAP(TRAP_INTOVF); \
	}

// ADD/SUB Results for Overflow bit
#define SetV_ADD(cc, res, src, add, sign) \
	if (~(src ^ add) & (res ^ add) & sign) setv();
#define SetV_SUB(cc, res, src, sub, sign) \
	if ((src ^ sub) & (res ^ ~sub) & sign) setv();

// ADD/SUB Results for Carry bit
#define SetC_ADD(cc, res, src)  \
	if (res < src) (cc) |= CC_C;
#define SetC_SUB(cc, src, sub)  \
	if (src < sub) (cc) |= CC_C;

// ADD Results for Condition Codes
#define SetCC_ADD_B(cc, res, src, add)   \
	SetNZ(cc, SXTB(res), 0, 0);          \
	SetC_ADD(cc, ZXTB(res), ZXTB(src));  \
	SetV_ADD(cc, res, src, add, SGN_BYTE);

#define SetCC_ADD_W(cc, res, src, add)   \
	SetNZ(cc, SXTW(res), 0, 0);          \
	SetC_ADD(cc, ZXTW(res), ZXTW(src));  \
	SetV_ADD(cc, res, src, add, SGN_WORD);

#define SetCC_ADD_L(cc, res, src, add)   \
	SetNZ(cc, SXTL(res), 0, 0);          \
	SetC_ADD(cc, ZXTL(res), ZXTL(src));  \
	SetV_ADD(cc, res, src, add, SGN_LONG);

// SUB Results for Condition Codes
#define SetCC_SUB_B(cc, res, src, sub)   \
	SetNZ(cc, SXTB(res), 0, 0);          \
	SetC_SUB(cc, ZXTB(src), ZXTB(sub));  \
	SetV_SUB(cc, res, src, sub, SGN_BYTE);

#define SetCC_SUB_W(cc, res, src, sub)   \
	SetNZ(cc, SXTW(res), 0, 0);          \
	SetC_SUB(cc, ZXTW(src), ZXTW(sub));  \
	SetV_SUB(cc, res, src, sub, SGN_WORD);

#define SetCC_SUB_L(cc, res, src, sub)   \
	SetNZ(cc, SXTL(res), 0, 0);          \
	SetC_SUB(cc, ZXTL(src), ZXTL(sub));  \
	SetV_SUB(cc, res, src, sub, SGN_LONG);

struct vaxOpcode {
	const char *opName;           // Name of the Instruction
	uint32_t    flags;            // Instruction Flags
	uint16_t    opCode;           // Extend/Opcode Value
	uint16_t    nOperands;        // Number of Operands
	uint16_t    oprMode[6];       // Attributes/Scales for Each Operand
};

// Decode opcode structure
struct dopc_t {
	const vaxOpcode *opCode;
	uint32_t flag;
	uint16_t nModes;
	uint16_t opMode[6];
};

struct tlb_t {
	uint32_t tag; // Tag ID number (VPN)
	uint32_t pte; // Process table entry
};

class vax_cpu_base : public cpu_device
{
public:
	vax_cpu_base(const system_config &config, const device_type_base &type,
			tag_t *tag, device_t *owner, uint64_t clock, uint8_t dWidth, uint8_t aWidth);
	virtual ~vax_cpu_base();

	void buildOpcodes();

	// CPU function calls
	virtual void init();
//	virtual void reset();
//	virtual int  boot();
	virtual void execute();

//	virtual void send(cpuSignal signal);

	virtual void devStart() override;
	virtual void devStop() override;
	virtual void devReset() override;

	int disasmOperand(uint32_t &vAddr, const vaxOpcode *opc, char **str, uint32_t &nWords);
//	int disasm(const cty_t *cty, uint32_t pcAddr);
	cmdStatus dump(const cty_t *cty, uint32_t *sAddr, uint32_t eAddr);

	int disassemble(const cty_t *cty, uint32_t vAddr);

	void assignMemory(uint8_t *mem, uint32_t memSize);
	void setPCAddr(uint32_t pcAddr);

protected:
	char *stringCC(uint32_t cc);
	int getBit();
	int setBit(int bit);
	int32_t getField(bool sign);
	void putField();

	void call(bool stkFlag);
	void ret();
	void movc(int c5flg);
	void cmpc(int c5flg);
	void locc(bool skpflg);
	void scanc(bool spnflg);
	void ldpctx();
	void svpctx();
	uint32_t probe(bool rwflg);
	virtual void mfpr() = 0;
	virtual void mtpr() = 0;

	// Interrupt/exception services
	int  evaluate();
	void interrupt();
	int  exception(int ie, uint32_t vec, uint32_t ipl);
	void emulate(uint32_t opc);
	int  fault(uint32_t vec);
	void faultfp(uint32_t fsts);
	void change(int mode, int32_t code);
	void resume();

	inline void storeb(uint32_t op, uint8_t data)
	{
		if (op > -CPU_nGREGS)
			gpReg[~op].b = data;
		else
			writev(op, data, LN_BYTE, WACC);
	}

	inline void storew(uint32_t op, uint16_t data)
	{
		if (op > -CPU_nGREGS)
			gpReg[~op].w = data;
		else
			writev(op, data, LN_WORD, WACC);
	}

	inline void storel(uint32_t op, uint32_t data)
	{
		if (op > -CPU_nGREGS)
			gpReg[~op].l = data;
		else
			writev(op, data, LN_LONG, WACC);
	}

	inline void storeq(uint32_t op, uint32_t wd1, uint32_t wd2)
	{
		if (op > -CPU_nGREGS) {
			gpReg[~op].l     = wd1;
			gpReg[(~op)+1].l = wd2;
		} else {
			writev(op+LN_LONG, wd2, LN_LONG, WACC);
			writev(op,         wd1, LN_LONG, WACC);
		}
	}

	inline void storeqp(uint32_t op, uint32_t *wd)
	{
		if (op > -CPU_nGREGS) {
			gpReg[~op].l     = wd[0];
			gpReg[(~op)+1].l = wd[1];
		} else {
			writev(op+LN_LONG, wd[1], LN_LONG, WACC);
			writev(op,         wd[0], LN_LONG, WACC);
		}
	}

	inline void storeo(uint32_t op, uint32_t *wd)
	{
		if (op > -CPU_nGREGS) {
			gpReg[~op].l     = wd[0];
			gpReg[(~op)+1].l = wd[1];
			gpReg[(~op)+2].l = wd[2];
			gpReg[(~op)+3].l = wd[3];
		} else {
			writev(op,    wd[0], LN_LONG, WACC);
			writev(op+4,  wd[1], LN_LONG, WACC);
			writev(op+8,  wd[2], LN_LONG, WACC);
			writev(op+12, wd[3], LN_LONG, WACC);
		}
	}

	inline void setv()
	{
		ccReg |= CC_V;
		if (psReg & PSW_IV) {
			irqFlags &= ~IRQ_TRAP;
			irqFlags |= IRQ_SETTRAP(TRAP_INTOVF);
		}
	}

public:
	// Memory access routines
	uint32_t readp(uint32_t addr, int size);                   // Read access (aligned)
	uint32_t readpl(uint32_t pAddr);                           // Longword read access (aligned)
	void     writep(uint32_t addr, uint32_t data, int size);   // Write access (aligned)
	void     writepl(uint32_t pAddr, uint32_t data);           // Longword write access (aligned)

	void     cleartlb(bool sysFlag);
	void     cleartlb(uint32_t vAddr);
	bool     checktlb(uint32_t vAddr);

	tlb_t    pagefault(uint32_t errCode, uint32_t vAddr, uint32_t acc, uint32_t *sts);
	tlb_t    translate(uint32_t vAddr, int size, uint32_t acc, uint32_t *sts = nullptr);
	uint32_t probev(uint32_t vAddr, uint32_t acc, uint32_t *sts);

	uint32_t readv(uint32_t vAddr, uint32_t size, uint32_t acc);
	void     writev(uint32_t vAddr, uint32_t data, uint32_t size, uint32_t acc);

	void     flushvi();
	uint32_t readvi(int size); // Instruction read access

	// Read/write privileged register access
	virtual uint32_t readpr(uint32_t pReg);
	virtual void     writepr(uint32_t pReg, uint32_t data);
	virtual void     halt(uint32_t code);
	virtual void     check(uint32_t code);

	// Console memory access routines
	uint32_t readpc(uint32_t pAddr, int size);
	void     writepc(uint32_t pAddr, uint32_t data, int size);
	uint32_t readc(uint32_t vAddr, int size, uint32_t *sts);
	void     writec(uint32_t vAddr, uint32_t data, int size, uint32_t *sts);

	di_memory::spaceConfigVector mapGetSpaceVector() const;

protected:
	scale32_t gpReg[CPU_nGREGS];  // General registers
	uint32_t  ipReg[CPU_nPREGS];  // Processor registers
	uint32_t  opReg[CPU_nOREGS];  // Operand registers
	int32_t   rqReg[CPU_nOREGS];  // Recovery registers
	uint32_t  paReg[CPU_nNREGS];  // Parameter registers

	uint32_t  psReg;               // Processor status register
	uint32_t  ccReg;               //    Condition Codes
	uint32_t  accMode;             //    Access Mode

	uint32_t  opCode;              // Current opcode
	uint32_t  opCount;             // Number of operand registers
	uint32_t  rqCount;             // Number of recovery registers
	uint32_t  paCount;             // Number of parameter registers

	uint32_t  flags;               // Processor flags

	// Machine check
	uint32_t  mchkAddr;
	uint32_t  mchkRef;

	// Opcode table for operand decoding and disassembler
	const vaxOpcode *opCodes[CPU_nOPCTBL];

	dopc_t dopCode[CPU_nOPCTBL]; // Decode opcode table

	static const uint32_t mskList[];
	static const uint32_t sgnList[];

	// Mirror of system memory (from system device)
	uint32_t  memSize;
	uint8_t  *mem;

	// Mapping address space configuration
	const mapAddressConfig mapProgramConfig;
	mapAddressSpace *mapProgram;

	// Instruction buffer (look-ahead buffer)
	uint32_t  ibData[2]; // IB Aligned data buffer
	uint32_t  ibpAddr;   // IB physical Address
	uint32_t  ibCount;   // IB Count

	// Memory management unit
	uint32_t  curMode;    // Current access mode
	uint32_t  prvMode;    // Previous access mode

	uint32_t  paMask;     // Physical addressing mask

	// Interrupt/exception services
	uint32_t  faultAddr;   // Faulting PC address
	uint32_t  scbCode;     // SCB code pending
	uint32_t  irqFlags;

	tlb_t     tlbSystem[TLB_SIZE];
	tlb_t     tlbProcess[TLB_SIZE];

	uint32_t  cfgFlag; // Configuration flags

#ifdef ENABLE_DEBUG
	Debug      dbg;
#endif /* ENABLE_DEBUG */
};
