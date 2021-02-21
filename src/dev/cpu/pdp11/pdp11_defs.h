/*
 * pdp11_defs.h - PDP11 processor device definitions package
 *
 *  Created on: Feb 21, 2021
 *      Author: Tim Stark
 */

#pragma once

#define CPU_nGREGS  8

// Register List
#define REG_nR0		0
#define REG_nR1		1
#define REG_nR2		2
#define REG_nR3		3
#define REG_nR4		4
#define REG_nR5		5
#define REG_nR6		6
#define REG_nR7		7

// PSW - Processor Status Register
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// | CM  | PM  |RS|0  0 |SI|   IPL  |T |N |Z |V |C |
// +--^--+--+--^--+--+--^--+--+--^--+--+--^--+--+--+
//  15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00

#define PSW_CM  0140000 // Current Mode
#define PSW_PM  0030000 // Previous Mode
#define PSW_RS  0004000 // General Register Set     (J11 only)
#define PSW_SI  0000400 // Supsended Information    (F11 only)
#define PSW_IPL 0000340 // Interrupt Priority Level
#define PSW_T   0000020 // Trace/Trap Mode
#define PSW_N   0000010 // Negative
#define PSW_Z   0000004 // Zero
#define PSW_V   0000002 // Overflow
#define PSW_C   0000001 // Carry
#define PSW_RW  0174357 // Writable Bits for J11

#define PSW_CC  (PSW_N|PSW_Z|PSW_V|PSW_C)

// Condition Code Flags
#define CC_SET 020     // Set bit in CC instructions
#define CC_N   PSW_N   // Negative Flag
#define CC_Z   PSW_Z   // Negative Flag
#define CC_V   PSW_V   // Overflow Flag
#define CC_C   PSW_C   // Carry Flag
#define CC_ALL (CC_N|CC_Z|CC_V|CC_C)

#define PSW_P_CM 14
#define PSW_P_PM 12
#define PSW_MODE 03

#define PSW_GETCUR(psw) ((psw >> PSW_P_CM) & PSW_MODE)
#define PSW_GETPRV(psw) ((psw >> PSW_P_PM) & PSW_MODE)

// Access Mode (Protection Codes)
#define AM_KERNEL  0  // Kernel mode
#define AM_SUPER   1  // Supervisor mode
#define AM_ILLEGAL 2  // Illegal mode
#define AM_USER    3  // User mode

// Floating Status Register
#define FPS_ER   0100000  // Error
#define FPS_ID   0040000  // Interrupt Disable
#define FPS_IUV  0004000  // Undefined Error Interrupt
#define FPS_IU   0002000  // Underflow Error Interrupt
#define FPS_IV   0001000  // Overflow Error Interrupt
#define FPS_IC   0000400  // IC Error Interrupt
#define FPS_D    0000200  // Double Precision
#define FPS_L    0000100  // Long Precision
#define FPS_T    0000040  // Truncate
#define FPS_CC   0000017  // Condition Codes
#define FPS_N    0000010  //    Negative
#define FPS_Z    0000004  //    Zero
#define FPS_V    0000002  //    Overflow
#define FPS_C    0000001  //    Carry

// PIRQ - Programmable IRQ Register
#define PIRQ_PIR7    0100000
#define PIRQ_PIR6    0040000
#define PIRQ_PIR5    0020000
#define PIRQ_PIR4    0010000
#define PIRQ_PIR3    0004000
#define PIRQ_PIR2    0002000
#define PIRQ_PIR1    0001000

#define PIRQ_RW      0177000
#define PIRQ_IMP     0177356

// STKLIM - Stack Limit Register
#define STKLIM_IMP   0177400

// Stack Pointer - Limits for Trap Routines
#define STKL_RED4   0004 // Default Red Stack
#define STKL_RED    0340 // Red Zone Area (optional)
#define STKL_YEL    0400 // Yellow Zone Area

#define p11_CheckStack(_cpu, _rn, _addr) \
	if (((_rn) == REG_nSP) && (PSW_GETCUR(_cpu->pswFlags) == ACC_KERNEL) && \
	    ((_addr) < (_cpu->stkLimit + STKL_YEL)))                            \
		p11cpu_SetStackTrap(_cpu, _addr)

#define UBM_CSRADR      0770200
#define UBM_NREGS       (UBM_SIZE << 1)

// I/O Mapping
#define UBM_SIZE        32
#define UBM_P_SIZE      13
#define UBM_PAGE        037
#define UBM_BLKSZ       01000
#define UBM_BLKOFF      (UBM_BLKSZ - 1)
#define UBA_OFF         017777
#define UBM_GETPN(pn)   (((pn) >> UBM_P_PAGE) & UBM_PAGE)
#define UBM_GETOFF(off) (((off) & UBM_OFF)


// Trap Mask - Desecending priority according to PDP-11 manuals
#define TRAP_P_RED   0  // Red Stack Error
#define TRAP_P_ODD   1  // Odd Address
#define TRAP_P_MME   2  // Memory Management Error
#define TRAP_P_NXM   3  // Non-Existant Memory
#define TRAP_P_PAR   4  // Parity Error
#define TRAP_P_PRV   5  // Privileged Instruction
#define TRAP_P_ILL   6  // Illegal Instruction
#define TRAP_P_BPT   7  // Breakpoint
#define TRAP_P_IOT   8  // I/O Instruction
#define TRAP_P_EMT   9  // Emulator Instruction
#define TRAP_P_TRAP  10 // Trap Instruction
#define TRAP_P_TRC   11 // Trace (T-bit)
#define TRAP_P_YEL   12 // Yellow Stack Error
#define TRAP_P_PWR   13 // Power Failure
#define TRAP_P_FPE   14 // Floating-Point Error
#define TRAP_P_INT   15 // Interrupt Requests

#define TRAP_RED  (1u << TRAP_P_RED)
#define TRAP_ODD  (1u << TRAP_P_ODD)
#define TRAP_MME  (1u << TRAP_P_MME)
#define TRAP_NXM  (1u << TRAP_P_NXM)
#define TRAP_PAR  (1u << TRAP_P_PAR)
#define TRAP_PRV  (1u << TRAP_P_PRV)
#define TRAP_ILL  (1u << TRAP_P_ILL)
#define TRAP_BPT  (1u << TRAP_P_BPT)
#define TRAP_IOT  (1u << TRAP_P_IOT)
#define TRAP_EMT  (1u << TRAP_P_EMT)
#define TRAP_TRAP (1u << TRAP_P_TRAP)
#define TRAP_TRC  (1u << TRAP_P_TRC)
#define TRAP_YEL  (1u << TRAP_P_YEL)
#define TRAP_PWR  (1u << TRAP_P_PWR)
#define TRAP_FPE  (1u << TRAP_P_FPE)
#define TRAP_INT  (1u << TRAP_P_INT)

#define TRAP_MAX  TRAP_INT
#define TRAP_ALL  (TRAP_MAX - 1) // All trap requests

#define VEC_RED     0004
#define VEC_ODD     0004
#define VEC_MME     0250
#define VEC_NXM     0004
#define VEC_PAR     0114
#define VEC_PRV     0004
#define VEC_ILL     0010
#define VEC_BPT     0014
#define VEC_IOT     0020
#define VEC_EMT     0030
#define VEC_TRAP    0034
#define VEC_TRC     0014
#define VEC_YEL     0004
#define VEC_PWR     0024
#define VEC_FPE     0244
#define VEC_PIRQ    0240

#define SetTraps(_cpu, _irq)      (_cpu->intReqs |= (_irq))
#define ClearTraps(_cpu, _irq)    (_cpu->intReqs &= ~(_irq))


// Virtual address definition
//
//          |<---------- 16-bit virtual address -------->|
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |Mode |DS|  APF   |       Base      |     Offset      |
// +--+--+--^--+--+--^--+--+--^--+--+--^--+--+--^--+--+--+
//  18 17 16 15 14 13 12 11 10 09 08 07 05 04 03 02 01 00

#define VA_WIDTH     16
#define VA_SIZE      (1u << VA_WIDTH)
#define VA_MASK      (VA_SIZE - 1)

#define VA_DSPACE    (1u << VA_P_DS)
#define VA_MODE      (1u << VA_P_MODE)
#define VA_DF        0017777
#define VA_BN        0017700
#define VA_P_APF     13
#define VA_P_DS      16
#define VA_P_MODE    17

// Physical address definition
#define PA_SIZE16    (1u << 16)   // 16-bit physical addressing
#define PA_MASK16    (PA_SIZE16 - 1)
#define PA_IOPAGE16  (PA_MASK16 ^ IO_MASK)

#define PA_SIZE18    (1u << 18)   // 18-bit physical addressing
#define PA_MASK18    (PA_SIZE18 - 1)
#define PA_IOPAGE18  (PA_MASK18 ^ IO_MASK)

#define PA_SIZE22    (1u << 22)   // 22-bit physical addressing
#define PA_MASK22    (PA_SIZE22 - 1)
#define PA_IOPAGE22  (PA_MASK22 ^ IO_MASK)

#define MEM_64K      (PA_SIZE16)
#define MEM_256K     (PA_SIZE18)
#define MEM_4M       (PA_SIZE22)

// I/O page addressing
#define IO_WIDTH     13
#define IO_SIZE      (1u << IO_WIDTH)
#define IO_MASK      (IO_SIZE - 1)
#define IO_PAGEBASE  PA_IOPAGE22
#define IO_PAGE16    017600000
#define IO_PAGE18    017000000



// I/O - Memory Management Unit
//
// Registers
// APRs        772300   16 (0040)
// APRs        777600   16 (0040)
// MMR3        772516    1 (0002)
// SR/MMR0-2   777570    4 (0010)

// Memory Management Registers
#define MMR_nREGS  4

// APR File Registers

#define APR_nREGS   64 // Number of PAR/PDR registers
#define APR_nREGS2  8  // Number of PAR/PDR registers
#define APR_nMODE   16 // Number of PAR/PDR registers
#define APR_KERNEL  0  // 16 PAR/PDR registers (kernel mode)
#define APR_SUPER   16 // 16 PAR/PDR registers (supervisor mode)
#define APR_UNUSED  32 // 16 PAR/PDR registers (reserved)
#define APR_USER    48 // 16 PAR/PDR registers (user mode)

#define MM0_nREGS   3
#define MM3_nREGS   1

// SR/MMR Registers
#define SR_CSRADR   017570
#define MM0_CSRADR  017572
#define MM3_CSRADR  012516
// APR Registers (Kernel Mode)
#define APR_KIPDR   012300
#define APR_KDPDR   012320
#define APR_KIPAR   012340
#define APR_KDPAR   012360
// APR Registers (Supervisor Mode)
#define APR_SIPDR   012200
#define APR_SDPDR   012220
#define APR_SIPAR   012240
#define APR_SDPAR   012260
// APR Registers (User Mode)
#define APR_UIPDR   017600
#define APR_UDPDR   017620
#define APR_UIPAR   017640
#define APR_UDPAR   017660

// MMR0 - Memory Management Register #0
#define MMR0_FREEZE  0160000 // No updates if any error bit set
#define MMR0_NR      0100000 //   Non-resident Error
#define MMR0_PL      0040000 //   Page Length Error
#define MMR0_RO      0020000 //   Read-Only Error
#define MMR0_MAI     0000400 // Maintenance (Not Implemented)
#define MMR0_IC      0000200 // Instruction Complete
#define MMR0_MODE    0000140 // Mode from PSW as during abort
#define MMR0_SPACE   0000020 // Address Space during abort
#define MMR0_PAGE    0000016 // Aborted Page during Abort
#define MMR0_MME     0000001 // Memory Management Enable
#define MMR0_RW      0160001 // Writable Mask

#define MMR0_M_PAGE  (MMR0_MODE|MMR0_SPACE|MMR0_PAGE)
#define MMR0_P_PAGE  1

// MMR3 - Memory Management Register #3
#define MMR3_BME     0000040 // DMA Bus Map Enable
#define MMR3_M22E    0000020 // 22-bit Memory Management Enable
#define MMR3_CSM     0000010 // CSM Instruction Enable
#define MMR3_KDS     0000004 // Kernel Data Space Enable
#define MMR3_SDS     0000002 // Supervisor Data Space Enable
#define MMR3_UDS     0000001 // User Data Space Enable
#define MMR3_RW      0000077 // Writable Mask

// PAR - Page Address Register
// PDR - Page Descriptor Register
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |              Page Address Register            |
// +--^--+--+--^--+--+--^--+--+--^--+--+--^--+--+--+
// |BY|        PLF         |A |W | 0 0 |ED|   ACF  |
// +--^--+--+--^--+--+--^--+--+--^--+--+--^--+--+--+
//  15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00

#define PAR_18B     0007777 // 18-bit Addressing Mask
#define PAR_22B     0177777 // 22-bit Addressing Mask

#define PDR_NOC     0100000 // Bypass Cache Bit (BY)
#define PDR_PLF     0077400 // Page Length Field
#define PDR_A       0000200 // Access Flag
#define PDR_W       0000100 // Written Flag
#define PDR_ED      0000010 // Expansion Direction
#define PDR_ACF     0000007 // Access Control Field (3-bit)
#define PDR_ACS     0000006 // Access Control Field (2-bit)
#define PDR_PWR     0000004 // Page Writable
#define PDR_PRD     0000003 // Page Readable
#define PDR_RW      0177416 // Writable Mask

// MAINT - Maintenance Register
#define MAINT_UQ     0001000 // Bus type (Qbus or Unibus)
#define MAINT_FPA    0000400 // Floating Point Accelerator
#define MAINT_TYPE   0000060 // Processor Type
#define MAINT_HTRAP  0000010 // Trap 4 on HALT Option
#define MAINT_POM    0000006 // Power On Option
#define MAINT_BPOK   0000001 // Power Ok

#define MAINT_P_POM  1
#define MAINT_P_TYPE 4

#define MAINT_KDJA  1        // KDJ11A Processor
#define MAINT_KDJB  2        // KDJ11B Processor
#define MAINT_KDJD  4        // KDJ11D Processor
#define MAINT_KDJE  5        // KDJ11E Processor

#define MAINT_POODT  0       // Power Up, ODT
#define MAINT_POROM  2       // Power Up, ROM

// CPUERR - Error Register
#define CPUE_BUSE   0004000 // Bus Error (11/44 only)
#define CPUE_HALT   0000200 // Illegal HALT (Not Kernel or HTRAP option)
#define CPUE_ODD    0000100 // Odd Address Error
#define CPUE_NXM    0000040 // Non-existant Memory
#define CPUE_TMO    0000020 // I/O Page NXM (Timeout) Error
#define CPUE_YEL    0000010 // Yellow Stack Error
#define CPUE_RED    0000004 // Red Stack Error

#define setCPUError(_err)   (cpuError |= (_err))
#define clearCPUError(_err) (cpuError &= ~(_err))


// System Option Feature Bits
#define OPT_M9312    0x80000000
#define OPT_RH11     0x40000000

#define BUS_22B      0x00100000
#define BUS_18B      0x00080000
#define BUS_16B      0x00040000
#define BUS_QB       0x00020000
#define BUS_UB       0x00010000

#define OPT_UBM      0x00008000
#define OPT_PAR      0x00004000
#define OPT_SID      0x00002000
#define OPT_MMU      0x00001000
#define OPT_FPP      0x00000800
#define OPT_CIS      0x00000400
#define OPT_FIS      0x00000200
#define OPT_EIS      0x00000100

#define OPT_J11E     0x00000080
#define OPT_J11D     0x00000040
#define OPT_J11B     0x00000020
#define OPT_J11A     0x00000010
#define OPT_J11      (OPT_J11A|OPT_J11B|OPT_J11D|OPT_J11E)
#define OPT_F11      0x00000008
#define OPT_T11      0x00000004
#define OPT_44       0
#define OPT_24       0

// Processor Option Feature Bits
#define OPT_IOEXE    0x00080000
#define OPT_IOPSW    0x00040000 // Has PSW register in I/O space
#define OPT_STKLR    0x00020000
#define OPT_STKLF    0x00010000
#define OPT_CHGTBIT  0x00004000 // Ability to change T bit
#define OPT_ODD      0x00002000 // Has odd address trap
#define OPT_SDSD     0x00001000
#define OPT_JREGA    0x00000800
#define OPT_JREG4    0x00000400
#define OPT_HALT4    0x00000200
#define OPT_TSWLK    0x00000100 // Has TSTSET/WRLCK instruction
#define OPT_CSM      0x00000080 // Has CSM instruction
#define OPT_SPL      0x00000040 // Has SPL instruction
#define OPT_RTT      0x00000020 // Has RTT instruction
#define OPT_MARK     0x00000010 // Has MARK instruction
#define OPT_SXS      0x00000008
#define OPT_MxPy     0x00000004 // Has MxPy instruction
#define OPT_MxPS     0x00000002 // Has MxPS instruction
#define OPT_MFPT     0x00000001 // Has MFPT instruction

struct pdp11_Opcode {
	const char *opName;
	uint32_t    opFlags;
	uint32_t    cpuFlags;
	uint32_t    optFlags;
	uint16_t    opCode;
	uint16_t    opMask;
};

//class pdp11_cpu : public cpu_device
//{
//public:
//	pdp11_cpu() {};
//	~pdp11_cpu() {};
//
//	virtual void devStart() override;
////	virtual void devStop() override;
////	virtual void devReset() override;
//
//protected:
//	uint32_t cpuFlag;  // CPU feature flags
//	uint32_t optFlag;  // Option feature flags
//
//	uint16_t wkReg[CPU_nGREGS];  // Working registers (R0-R7, SP and PC)
//	uint16_t gpReg[6][2];        // register set (R0-R5, R0'-R5')
//	uint16_t stkReg[4];          // Stack registers (KSP, SSP, and USP)
//	uint16_t stkLimit;           // Stack limit register
//
//	uint16_t intReq;        // Interrupt/trap requests
//	uint16_t intVec[16];    // Interrupt/trap vectors
//	uint16_t intFlags;      // Trap in progress flag
//	uint16_t intAddr;       // Current trap vector address
//
//	uint16_t opCode;        // Instruction Register        (IR)
//	uint16_t psMask;        // PSW Writable Mask           (Specific-Model)
//	uint16_t psFlag;        // Processor Status Word       (PSW)
//	uint16_t ccFlag;        //   Condition Codes
//	uint16_t pgmReqs;       // Program Interrupt Requests  (PIRQ)
//	uint16_t cpuMaint;      // CPU Maintenance Register    (MAINT)
//	uint16_t cpuType;       // CPU Identification          (CPU ID)
//	uint16_t cpuError;      // CPU Error Register          (CPUER)
//	uint16_t memError;      // Memory Error Register       (MEMERR)
//	uint16_t cacReg;        // Cache Control Register      (CCR)
//	uint16_t cacHit;        // Hit/Miss Register           (HITMISS)
//
//	uint16_t csReg;         // Control/Status Register     (CSR)
//	uint16_t pcReg;         // Page Control Register       (PCR)
////	uint16_t padReg;        // Maintenance Register        (MAINT)
//	uint16_t swReg;         // Configuration Register      (SR)
//	uint16_t dpReg;         // Display Register            (DR)
//	uint16_t asReg;         // Additional Status Register  (ASR)
//	uint16_t romPage73;     // ROM Page 0 Offset Address
//	uint16_t romPage65;     // ROM Page 1 Offset Address
//
//	uint32_t iSpace;        // Instruction Space
//	uint32_t dSpace;        // Data Space
//
//	// Memory management registers
//	uint16_t mmReg[MMR_nREGS];     // Memory management registers (MMR0-MMR3)
//	uint16_t mm0Mask;              // MMR0 Writable Mask (Specific-Model)
//	uint16_t mm3Mask;              // MMR3 Writable Mask (Specific-Model)
//	uint16_t parMask;              // PAR Writable Mask (Specific-Model)
//	uint16_t pdrMask;              // PDR Writable Mask (Specific-Model)
//	uint16_t parFile[APR_nREGS];
//	uint16_t pdrFile[APR_nREGS];
//
//	// I/O Mapping for CPU registers
////	uint16_t apReg[APR_nREGS];   // APR file registers
//};
