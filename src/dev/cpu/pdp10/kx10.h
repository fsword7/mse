/*
 * kx10.h - PDP-10 family processor package
 *
 *  Created on: Sep 6, 2015
 *      Author: Tim Stark
 *
 *  Digital Equipment Corporation - 36-bit PDP-10 family systems
 *
 *  name	MIPS	Year	Descriptions
 *
 *  PDP-6	0.25	1964	Original 36-bit implementation
 *  KA10	0.38	1967	First PDP-10, flip chips
 *  KI10	0.72	1972	FIrst paging system, flip chips + MSI
 *  KL10A	1.8		1975	First ECL system, ECL 10K
 *  KL10B	1.8		1978	Extended addressing, ECL 10K
 *  KS10	0.3		1979	2901-based chips, last 36-bit system
 *
 *
 *	In additional, it ran one of major operating systems
 *
 *	TOPS-10		Original timesharing system
 *	TOPS-20		Commercial version of TENEX
 *	ITS			Incompatible timesharing system
 *	TENEX		ARPA-sponsored timesharing system
 *
 */

#pragma once

#include "emu/devcpu.h"
#include "formats/dec/word10.h"

#define AC_NBLKS      8
#define AC_NREGS    020

// Define only one of CPU types for compiling time
#define CPU_KS10 1
#define OS_DEC 1

// PDP6 processor
#ifndef CPU_PDP6
#define CPU_PDP6 0
#endif

// KA10 processor
#ifndef CPU_KA10
#define CPU_KA10 0
#endif

// KI10 processor
#ifndef CPU_KI10
#define CPU_KI10 0
#endif

// KL10A processor
#ifndef CPU_KL10A
#define CPU_KL10A 0
#endif

// KL10B processor with extended addressing
#ifndef CPU_KL10B
#define CPU_KL10B 0
#endif

// KL10 processor - either KL10A or KL10B
#ifndef CPU_KL10
#define CPU_KL10 (CPU_KL10A|CPU_KL10B)
#endif

// KS10 processor
#ifndef CPU_KS10
#define CPU_KS10 0
#endif

#if (CPU_PDP6 + CPU_KA10 + CPU_KI10 + CPU_KL10 + CPU_KS10) != 1
#error "Please define only one of CPU types - aborted"
#endif

#ifndef KI10_22BIT
#define KI10_22BIT (CPU_KI10|CPU_KL10)
#endif

// DEC operating systems (TOPS-10 and TOPS-20)
#ifndef OS_DEC
#define OS_DEC 0
#endif

// MIT Incomputable Timesharing System
#ifndef OS_ITS
#define OS_ITS 0
#endif

#if (OS_DEC + OS_ITS) != 1
#error "Please define only one of OS types - aborted"
#endif

// Determine maximum memory size for one of PDP-10 processors
#if KI10_22BIT|CPU_KI10
#define MEM_MAXSIZE (4096 * 1024)
#else
#if CPU_PDP6
#define MEM_MAXSIZE (256 * 1024)
#else
#define MEM_MAXSIZE (1024 * 1024)
#endif
#endif

// Determine address width for one of PDP-10 processors
#if KL10_22BIT
#define PADR_WIDTH	22
#define PADR_MASK	017777777LL
#define CTX_MASK	0037777LL
#else
#define PADR_WIDTH	18
#define PADR_MASK	W10_RMASK
#define CTX_MASK	W10_RMASK
#endif

// PC flags (left halfword)
#define PCF_AOV			0400000 // Arithmetic overflow
#define PCF_PCH			0400000 // PC changing (PDP6)
#define PCF_PCP			0400000 // Previous context public
#define PCF_FOV			0040000 // Floating overflow
#define PCF_CR0			0200000 // Carry #0
#define PCF_CR1			0100000 // Carry #1
#define PCF_FPD			0020000 // First part done
#define PCF_USR			0010000 // User mode
#define PCF_UIO			0004000 // User I/O mode
#define PCF_PCU			0004000 // Previous context user
#define PCF_PUB			0002000 // Public mode
#define PCF_AFI			0001000 // Address failure inhibit
#define PCF_1PR			0001000 // 1-Proceed flag (ITS)
#define PCF_TR2			0000400 // Trap #2 (Stack trap)
#define PCF_TR1			0000200 // Trap #1 (arithmetic trap)
#define PCF_FXU			0000100 // Floating underflow
#define PCF_DCX			0000040 // NO divide (divide check)

#define PCF_FLAGS		0777740 // PC flags field
#define PCF_SECTION		0000037 // PC section field
#define PCF_TRAPS		(PCF_TR1|PCF_TR2)


// Instruction Format
//
// |<---------- Left Halfword -------->|<--------- Right Halfword -------->|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |      Opcode     |   AC  |I|   X   |            Address (Y)            |
// +-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-+
//  0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5

// Extended Format Instruction Format
//
// |<---------- Left Halfword -------->|<--------- Right Halfword -------->|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |0|I|   X   |                      Address (Y)                          |
// +-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-+
//  0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5

// I/O Instruction Format
//
// |<---------- Left Halfword -------->|<--------- Right Halfword -------->|
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |1 1 1|   Device    | FNC |I|   X   |            Address (Y)            |
// +-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-^-+-+-+
//  0 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3
//  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5

#define OPC_MASK 0777 // Opcode mask
#define DEV_MASK 0177 // Device Code Mask
#define FNC_MASK 07   // Function Code Mask
#define AC_MASK  017  // AC mask

// Function Code Definitions
#define IOF_BLKI   0 // (00) Block In
#define IOF_DATAI  1 // (04) Data In
#define IOF_BLKO   2 // (10) Block Out
#define IOF_DATAO  3 // (14) Data Out
#define IOF_CONO   4 // (20) Conditions Out
#define IOF_CONI   5 // (24) Conditions In
#define IOF_CONSZ  6 // (30) Conditions Skip Zero
#define IOF_CONSO  7 // (34) Conditions Skip Ones

#define IW_OP  0777000000000LL // LH: Opcode Field
#define IW_DEV 0077400000000LL // LH: Device Field
#define IW_FNC 0000340000000LL // LH: Function Field
#define IW_AC  0000740000000LL // LH: Accumulator Field
#define IW_I   0000020000000LL // LH: Indirect Field
#define IW_X   0000017000000LL // LH: Index Register Field
#define IW_Y   0000000777777LL // RH: Address Field

#define IW_GETOP(w)  (((w) >> 27) & OPC_MASK)
#define IW_GETDEV(w) (((w) >> 26) & DEV_MASK)
#define IW_GETFNC(w) (((w) >> 23) & FNC_MASK)
#define IW_GETAC(w)  (((w) >> 23) & AC_MASK)
#define IW_GETIX(w)  (((w) >> 18) & AC_MASK)
#define IW_GETI(w)   ((w) & IW_I)
#define IW_GETX(w)   ((w) & IW_X)
#define IW_GETY(w)   ((w) & IW_Y)

// Operand flags
#define OPF_FMEM	0000001	//  Fetch memory into AR
#define OPF_FSMEM	0000002 //  Fetch and save memory into AR
#define OPF_SMEM	0000004 //  Save AR into memory
#define OPF_FAC		0000010 //  Copy AR to BR then fetch AC into AR
#define OPF_FAC2	0000020 //  Fetch AC+1 into MQ
#define OPF_FBR     0000040 //  Fetch AC into BR
#define OPF_SAC		0000100 //  Save AR into AC
#define OPF_SACZ	0000200 //  Save AR into AC if AC is not zero
#define OPF_SAC2	0000400 //  Save MQ into AC+1
#define OPF_SWAR    0001000 //  Swap AR

// AC flags for PXCT instruction
#define PXCT_XEA   010 // Effective Address Calc     (PXCT bit 9)
#define PXCT_XRW   004 // Data Read/Write Access     (PXCT bit 10)
#define PXCT_XBEA  002 // BP Effective Address Calc  (PXCT bit 11)
#define PXCT_XBRW  001 // BP Data Read/Write Access  (PXCT bit 10)

class pdp10_cpu_base : public cpu_device
{
public:
	pdp10_cpu_base() = default;
	virtual ~pdp10_cpu_base() = default;

//	virtual void devStart() override;
//	virtual void devStop() override;
//	virtual void devReset() override;

	virtual void execute();

	int readp();  // physical read-access  (no paging)
	int writep(); // physical write-access (no paging)
	int readv(uint32_t ctx, uint32_t flags); // virtual read-access
	int writev(uint32_t ctx, uint32_t flags);  // virtual write-access

protected:

	// Accumulators
	w10_t  acRegs[AC_NBLKS][AC_NREGS];
	// Current/Previous Accumulators
	w10_t *curReg = nullptr; // Current AC block
	w10_t *prvReg = nullptr; // Previous AC block

	// Internal data registers
	w10_t arReg = 0; // AR register
	w10_t mqReg = 0; // Extension to AR register
	w10_t brReg = 0; // BR register
	w10_t mbReg = 0; // Memory data buffer
	w10_t abReg = 0; // Memory address buffer
	w10_t adReg = 0;
	uint32_t pcReg = 0; // Current PC address

	uint32_t memSize = 0;
	bool nxmFlag = false;
	h10_t pcFlags = 0; // Current PC flags

	w10_t *mem = nullptr;


	static uint32_t oprFlags[01000];
};
