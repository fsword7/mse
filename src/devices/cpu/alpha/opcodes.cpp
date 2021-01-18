/*
 * opcodes.cpp - AXP opcode tables
 *
 *  Created on: Jan 17, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "devices/cpu/alpha/axp.h"
#include "devices/cpu/alpha/debug.h"
#include "devices/cpu/alpha/opcodes.h"

// Instruction Formats for Alpha Processor Series
//
//   PALcode Instruction Format
//
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |  Opcode   |                   PALcode Number                  |
//   +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
//   Branch Instruction Format
//
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |  Opcode   |    Ra   |           21-bit Displacement           |
//   +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
//   Memory Instruction Format
//
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |  Opcode   |    Ra   |    Rb   |           Function            |
//   +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |  Opcode   |    Ra   |    Rb   |      16-bit  Displacement     |
//   +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
//   Operate Instruction Format
//
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |  Opcode   |    Ra   |    Rb   |       Function      |    Rc   |
//   +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |  Opcode   |    Ra   |    Rb   | SBZ |0|  Function   |    Rc   |
//   +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |  Opcode   |    Ra   |    Literate   |1|  Function   |    Rc   |
//   +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
//   Floating-Point Instruction Format
//
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |  Opcode   |    Fa   |    Fb   |       Function      |    Fc   |
//   +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

// Opcode Types
#define OP_VMS  CLS_PAL|OPR_TYPE|OPR_VMS
#define OP_UNX  CLS_PAL|OPR_TYPE|OPR_UNX
#define OP_WNT  CLS_PAL|OPR_TYPE|OPR_WNT
#define OP_PAL  CLS_PAL|OPR_PAL
#define OP_GRP  CLS_GRP|OPR_FUNC
#define OP_BRA  CLS_BRA|OPR_RA|OPR_BDP
#define OP_MJP  CLS_MEM|OPR_RA|OPR_RB|OPR_MJP
#define OP_MFC  CLS_MEM|OPR_RA|OPR_RB
#define OP_MFC1 CLS_MEM
#define OP_MEM  CLS_MEM|OPR_RA|OPR_RB|OPR_MDP
#define OP_IPR  CLS_MEM|OPR_RA|OPR_MDP|OPR_IPR
#define OP_IOP  CLS_IOP|OPR_RA|OPR_RB|OPR_RC
#define OP_FOP  CLS_FOP|OPR_RA|OPR_RB|OPR_RC

#define INST(name, flags, op, fn) { name, flags, op, fn }

opcAlpha AlphaProcessor::axp_opCodes[] =
{
	// Branch instructions
	INST("BR",            OP_BRA, 0x30, 0 ),
	INST("BSR",           OP_BRA, 0x34, 0 ),

	INST("BEQ",           OP_BRA, 0x39, 0),
	INST("BLT",           OP_BRA, 0x3A, 0),
	INST("BLE",           OP_BRA, 0x3B, 0),
	INST("BNE",           OP_BRA, 0x3D, 0),
	INST("BGE",           OP_BRA, 0x3E, 0),
	INST("BGT",           OP_BRA, 0x3F, 0),
	INST("BLBC",          OP_BRA, 0x38, 0),
	INST("BLBS",          OP_BRA, 0x3C, 0),

	INST("FBEQ",          OP_BRA, 0x31, 0),
	INST("FBLT",          OP_BRA, 0x32, 0),
	INST("FBLE",          OP_BRA, 0x33, 0),
	INST("FBNE",          OP_BRA, 0x35, 0),
	INST("FBGE",          OP_BRA, 0x36, 0),
	INST("FBGT",          OP_BRA, 0x37, 0),

	INST("LDA",           OP_MEM, 0x08, 0),
	INST("LDAH",          OP_MEM, 0x09, 0),
	INST("LDBU",          OP_MEM, 0x0A, 0),
	INST("LDQ_U",         OP_MEM, 0x0B, 0),
	INST("LDWU",          OP_MEM, 0x0C, 0),
	INST("STW",           OP_MEM, 0x0D, 0),
	INST("STB",           OP_MEM, 0x0E, 0),
	INST("STQ_U",         OP_MEM, 0x0F, 0),

	INST("LDF",           OP_MEM, 0x20, 0),
	INST("LDG",           OP_MEM, 0x21, 0),
	INST("LDS",           OP_MEM, 0x22, 0),
	INST("LDT",           OP_MEM, 0x23, 0),
	INST("STF",           OP_MEM, 0x24, 0),
	INST("STG",           OP_MEM, 0x25, 0),
	INST("STS",           OP_MEM, 0x26, 0),
	INST("STT",           OP_MEM, 0x27, 0),
	INST("LDL",           OP_MEM, 0x28, 0),
	INST("LDQ",           OP_MEM, 0x29, 0),
	INST("LDL_L",         OP_MEM, 0x2A, 0),
	INST("LDQ_L",         OP_MEM, 0x2B, 0),
	INST("STL",           OP_MEM, 0x2C, 0),
	INST("STL_C",         OP_MEM, 0x2D, 0),
	INST("STQ",           OP_MEM, 0x2E, 0),
	INST("STQ_C",         OP_MEM, 0x2F, 0),

	// Logical instructions
	INST("CMOVEQ",        OP_IOP, 0x11, 0x24),
	INST("CMOVLT",        OP_IOP, 0x11, 0x44),
	INST("CMOVLE",        OP_IOP, 0x11, 0x64),
	INST("CMOVNE",        OP_IOP, 0x11, 0x26),
	INST("CMOVGE",        OP_IOP, 0x11, 0x46),
	INST("CMOVGT",        OP_IOP, 0x11, 0x66),
	INST("CMOVLBS",       OP_IOP, 0x11, 0x14),
	INST("CMOVLBC",       OP_IOP, 0x11, 0x16),
	INST("BIC",           OP_IOP, 0x11, 0x08),
	INST("BIS",           OP_IOP, 0x11, 0x20),
	INST("AMASK",         OP_IOP, 0x11, 0x61),
	INST("AND",           OP_IOP, 0x11, 0x00),
	INST("IMPLVER",       OP_IOP, 0x11, 0x6C),
	INST("ORNOT",         OP_IOP, 0x11, 0x28),
	INST("EQV",           OP_IOP, 0x11, 0x48),
	INST("XOR",           OP_IOP, 0x11, 0x40),


	// Arithmetic instructions
	INST("ADDL",          OP_IOP, 0x10, 0x00),
	INST("ADDL/V",        OP_IOP, 0x10, 0x40),
	INST("ADDQ",          OP_IOP, 0x10, 0x20),
	INST("ADDQ/V",        OP_IOP, 0x10, 0x60),
	INST("SUBL",          OP_IOP, 0x10, 0x09),
	INST("SUBL/V",        OP_IOP, 0x10, 0x49),
	INST("SUBQ",          OP_IOP, 0x10, 0x29),
	INST("SUBQ/V",        OP_IOP, 0x10, 0x69),

	INST("S4ADDL",        OP_IOP, 0x10, 0x02),
	INST("S8ADDL",        OP_IOP, 0x10, 0x12),
	INST("S4ADDQ",        OP_IOP, 0x10, 0x22),
	INST("S8ADDQ",        OP_IOP, 0x10, 0x32),

	INST("S4SUBL",        OP_IOP, 0x10, 0x08),
	INST("S8SUBL",        OP_IOP, 0x10, 0x18),
	INST("S4SUBQ",        OP_IOP, 0x10, 0x28),
	INST("S8SUBQ",        OP_IOP, 0x10, 0x38),

	INST("CMPBGE",        OP_IOP, 0x10, 0x0F),
	INST("CMPULT",        OP_IOP, 0x10, 0x1D),
	INST("CMPEQ",         OP_IOP, 0x10, 0x2D),
	INST("CMPULE",        OP_IOP, 0x10, 0x3D),
	INST("CMPLT",         OP_IOP, 0x10, 0x4D),
	INST("CMPLE",         OP_IOP, 0x10, 0x6D),


	INST("EXTBL",         OP_IOP, 0x12, 0x06),
	INST("EXTLH",         OP_IOP, 0x12, 0x6A),
	INST("EXTLL",         OP_IOP, 0x12, 0x26),
	INST("EXTQH",         OP_IOP, 0x12, 0x7A),
	INST("EXTQL",         OP_IOP, 0x12, 0x36),
	INST("EXTWH",         OP_IOP, 0x12, 0x5A),
	INST("EXTWL",         OP_IOP, 0x12, 0x16),

	INST("INSBL",         OP_IOP, 0x12, 0x08),
	INST("INSLH",         OP_IOP, 0x12, 0x67),
	INST("INSLL",         OP_IOP, 0x12, 0x28),
	INST("INSQH",         OP_IOP, 0x12, 0x77),
	INST("INSQL",         OP_IOP, 0x12, 0x38),
	INST("INSWH",         OP_IOP, 0x12, 0x57),
	INST("INSWL",         OP_IOP, 0x12, 0x18),

	INST("MSKBL",         OP_IOP, 0x12, 0x02),
	INST("MSKLH",         OP_IOP, 0x12, 0x62),
	INST("MSKLL",         OP_IOP, 0x12, 0x22),
	INST("MSKQH",         OP_IOP, 0x12, 0x72),
	INST("MSKQL",         OP_IOP, 0x12, 0x32),
	INST("MSKWH",         OP_IOP, 0x12, 0x52),
	INST("MSKWL",         OP_IOP, 0x12, 0x12),

	INST("SLL",           OP_IOP, 0x12, 0x39),
	INST("SRA",           OP_IOP, 0x12, 0x3C),
	INST("SRL",           OP_IOP, 0x12, 0x34),

	INST("ZAP",           OP_IOP, 0x12, 0x30),
	INST("ZAPNOT",        OP_IOP, 0x12, 0x31),


	INST("MULL",          OP_IOP, 0x13, 0x00),
	INST("MULL/V",        OP_IOP, 0x13, 0x40),
	INST("MULQ",          OP_IOP, 0x13, 0x20),
	INST("MULQ/V",        OP_IOP, 0x13, 0x60),
	INST("UMULH",         OP_IOP, 0x13, 0x30),


	INST("TRAPB",         OP_MFC, 0x18, 0x0000),
	INST("EXCB",          OP_MFC, 0x18, 0x0400),
	INST("MB",            OP_MFC1, 0x18, 0x4000),
	INST("WMB",           OP_MFC, 0x18, 0x4400),
	INST("FETCH",         OP_MFC, 0x18, 0x8000),
	INST("FETCH_M",       OP_MFC, 0x18, 0xA000),
	INST("RPCC",          OP_MFC, 0x18, 0xC000),
	INST("RC",            OP_MFC, 0x18, 0xE000),
	INST("ECB",           OP_MFC, 0x18, 0xE800),
	INST("RS",            OP_MFC, 0x18, 0xF000),
	INST("WH64",          OP_MFC, 0x18, 0xF800),


	INST("CLTZ",          OP_IOP, 0x1C, 0x32),
	INST("CTPOP",         OP_IOP, 0x1C, 0x30),
	INST("CTTZ",          OP_IOP, 0x1C, 0x33),
	INST("FTOIS",         OP_IOP, 0x1C, 0x7B),
	INST("FTOIT",         OP_IOP, 0x1C, 0x70),

	INST("MAXSB8",        OP_IOP, 0x1C, 0x3E),
	INST("MAXSW4",        OP_IOP, 0x1C, 0x3F),
	INST("MAXUB8",        OP_IOP, 0x1C, 0x3C),
	INST("MAXUW4",        OP_IOP, 0x1C, 0x3D),

	INST("MINSB8",        OP_IOP, 0x1C, 0x38),
	INST("MINSW4",        OP_IOP, 0x1C, 0x39),
	INST("MINUB8",        OP_IOP, 0x1C, 0x3A),
	INST("MINUW4",        OP_IOP, 0x1C, 0x3B),

	INST("PERR",          OP_IOP, 0x1C, 0x31),

	INST("SEXTB",         OP_IOP, 0x1C, 0x00),
	INST("SEXTW",         OP_IOP, 0x1C, 0x01),

	INST("PKLB",          OP_IOP, 0x1C, 0x37),
	INST("PKWB",          OP_IOP, 0x1C, 0x36),
	INST("UNPKLB",        OP_IOP, 0x1C, 0x35),
	INST("UNPKWB",        OP_IOP, 0x1C, 0x34),


	INST("JMP",           OP_MJP, 0x1A, 0x00),
	INST("JSR",           OP_MJP, 0x1A, 0x01),
	INST("RET",           OP_MJP, 0x1A, 0x02),
	INST("JSR_COROUTINE", OP_MJP, 0x1A, 0x03),

	// Integer/Floating instructions
	INST("ITOFF",         OP_FOP, 0x14, 0x014),
	INST("ITOFS",         OP_FOP, 0x14, 0x004),
	INST("ITOFT",         OP_FOP, 0x14, 0x024),

	// VAX Floating-Point Instructions
	INST("SQRTF/C",       OP_FOP, 0x14, 0x00A),
	INST("SQRTF",         OP_FOP, 0x14, 0x08A),
	INST("SQRTF/UC",      OP_FOP, 0x14, 0x10A),
	INST("SQRTF/U",       OP_FOP, 0x14, 0x18A),
	INST("SQRTF/SC",      OP_FOP, 0x14, 0x40A),
	INST("SQRTF/S",       OP_FOP, 0x14, 0x48A),
	INST("SQRTF/SUC",     OP_FOP, 0x14, 0x50A),
	INST("SQRTF/SU",      OP_FOP, 0x14, 0x58A),

	INST("SQRTG/C",       OP_FOP, 0x14, 0x02A),
	INST("SQRTG",         OP_FOP, 0x14, 0x0AA),
	INST("SQRTG/UC",      OP_FOP, 0x14, 0x12A),
	INST("SQRTG/U",       OP_FOP, 0x14, 0x1AA),
	INST("SQRTG/SC",      OP_FOP, 0x14, 0x42A),
	INST("SQRTG/S",       OP_FOP, 0x14, 0x4AA),
	INST("SQRTG/SUC",     OP_FOP, 0x14, 0x52A),
	INST("SQRTG/SU",      OP_FOP, 0x14, 0x5AA),

	INST("ADDF",          OP_FOP, 0x15, 0x080),
	INST("ADDG",          OP_FOP, 0x15, 0x0A0),

	INST("SUBF",          OP_FOP, 0x15, 0x081),
	INST("SUBG",          OP_FOP, 0x15, 0x0A1),

	INST("DIVF",          OP_FOP, 0x15, 0x083),
	INST("DIVG",          OP_FOP, 0x15, 0x0A3),

	INST("MULF",          OP_FOP, 0x15, 0x082),
	INST("MULG",          OP_FOP, 0x15, 0x0A2),

	INST("CMPGEQ",        OP_FOP, 0x15, 0x0A5),
	INST("CMPGLT",        OP_FOP, 0x15, 0x0A6),
	INST("CMPGLE",        OP_FOP, 0x15, 0x0A7),

	INST("CVTDG",         OP_FOP, 0x15, 0x09E),
	INST("CVTDG",         OP_FOP, 0x15, 0x09E),
	INST("CVTGD",         OP_FOP, 0x15, 0x0AD),
	INST("CVTGF",         OP_FOP, 0x15, 0x0AC),
	INST("CVTGQ",         OP_FOP, 0x15, 0x0AF),
	INST("CVTQF",         OP_FOP, 0x15, 0x0BC),
	INST("CVTQG",         OP_FOP, 0x15, 0x0BE),


	// IEEE Floating-Point Instructions
	INST("SQRTS/C",       OP_FOP, 0x14, 0x00B),
	INST("SQRTS/M",       OP_FOP, 0x14, 0x04B),
	INST("SQRTS",         OP_FOP, 0x14, 0x08B),
	INST("SQRTS/D",       OP_FOP, 0x14, 0x0CB),
	INST("SQRTS/UC",      OP_FOP, 0x14, 0x10B),
	INST("SQRTS/UM",      OP_FOP, 0x14, 0x14B),
	INST("SQRTS/U",       OP_FOP, 0x14, 0x18B),
	INST("SQRTS/UD",      OP_FOP, 0x14, 0x1CB),
	INST("SQRTS/SUC",     OP_FOP, 0x14, 0x50B),
	INST("SQRTS/SUM",     OP_FOP, 0x14, 0x54B),
	INST("SQRTS/SU",      OP_FOP, 0x14, 0x58B),
	INST("SQRTS/SUD",     OP_FOP, 0x14, 0x5CB),
	INST("SQRTS/SUIC",    OP_FOP, 0x14, 0x70B),
	INST("SQRTS/SUIM",    OP_FOP, 0x14, 0x74B),
	INST("SQRTS/SUI",     OP_FOP, 0x14, 0x78B),
	INST("SQRTS/SUID",    OP_FOP, 0x14, 0x7CB),

	INST("SQRTT/C",       OP_FOP, 0x14, 0x02B),
	INST("SQRTT/M",       OP_FOP, 0x14, 0x06B),
	INST("SQRTT",         OP_FOP, 0x14, 0x0AB),
	INST("SQRTT/D",       OP_FOP, 0x14, 0x0EB),
	INST("SQRTT/UC",      OP_FOP, 0x14, 0x12B),
	INST("SQRTT/UM",      OP_FOP, 0x14, 0x16B),
	INST("SQRTT/U",       OP_FOP, 0x14, 0x1AB),
	INST("SQRTT/UD",      OP_FOP, 0x14, 0x1EB),
	INST("SQRTT/SUC",     OP_FOP, 0x14, 0x52B),
	INST("SQRTT/SUM",     OP_FOP, 0x14, 0x56B),
	INST("SQRTT/SU",      OP_FOP, 0x14, 0x5AB),
	INST("SQRTT/SUD",     OP_FOP, 0x14, 0x5EB),
	INST("SQRTT/SUIC",    OP_FOP, 0x14, 0x72B),
	INST("SQRTT/SUIM",    OP_FOP, 0x14, 0x76B),
	INST("SQRTT/SUI",     OP_FOP, 0x14, 0x7AB),
	INST("SQRTT/SUID",    OP_FOP, 0x14, 0x7EB),

	INST("ADDS",          OP_FOP, 0x16, 0x080),
	INST("ADDT",          OP_FOP, 0x16, 0x0A0),

	INST("SUBS",          OP_FOP, 0x16, 0x081),
	INST("SUBT",          OP_FOP, 0x16, 0x0A1),

	INST("DIVS",          OP_FOP, 0x16, 0x083),
	INST("DIVT",          OP_FOP, 0x16, 0x0A3),

	INST("MULS",          OP_FOP, 0x16, 0x082),
	INST("MULT",          OP_FOP, 0x16, 0x0A2),

	INST("CMPTEQ",        OP_FOP, 0x16, 0x0A5),
	INST("CMPTLE",        OP_FOP, 0x16, 0x0A7),
	INST("CMPTLT",        OP_FOP, 0x16, 0x0A6),
	INST("CMPTUN",        OP_FOP, 0x16, 0x0A4),

	INST("CVTQS",         OP_FOP, 0x16, 0x0BC),
	INST("CVTQT",         OP_FOP, 0x16, 0x0BE),
	INST("CVTST",         OP_FOP, 0x16, 0x2AC),
	INST("CVTTQ",         OP_FOP, 0x16, 0x0AF),
	INST("CVTTS",         OP_FOP, 0x16, 0x0AC),

	INST("CPYS",          OP_FOP, 0x17, 0x020),
	INST("CPYSE",         OP_FOP, 0x17, 0x022),
	INST("CPYSN",         OP_FOP, 0x17, 0x021),
	INST("CVTLQ",         OP_FOP, 0x17, 0x010),
	INST("CVTQL",         OP_FOP, 0x17, 0x030),

	INST("FCMOVEQ",       OP_FOP, 0x17, 0x02A),
	INST("FCMOVGE",       OP_FOP, 0x17, 0x02D),
	INST("FCMOVGT",       OP_FOP, 0x17, 0x02F),
	INST("FCMOVLE",       OP_FOP, 0x17, 0x02E),
	INST("FCMOVLT",       OP_FOP, 0x17, 0x02C),
	INST("FCMOVNE",       OP_FOP, 0x17, 0x02B),

	INST("MF_FPCR",       OP_FOP, 0x17, 0x025),
	INST("MT_FPCR",       OP_FOP, 0x17, 0x024),

	// Hardware instructions
	INST("CALL_PAL",      OP_PAL, 0x00, 0),
	INST("HW_LD",         OP_MEM, 0x1B, 0),
	INST("HW_ST",         OP_MEM, 0x1F, 0),
	INST("HW_MFPR",       OP_IPR, 0x19, 0),
	INST("HW_MTPR",       OP_IPR, 0x1D, 0),
	INST("HW_REI",        OP_MEM, 0x1E, 0),


	// Null terminator
	nullptr
};
