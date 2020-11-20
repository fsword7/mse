#include "emu/core.h"
#include "emu/debug.h"
#include "dev/cpu/alpha/axp.h"

#if 0
// Instruction Formats for Alpha Processors
//
//   PALcode Instrcution Format
//
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |  Opcode   |                       Number                      |
//   +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
//   Branch Instruction Format
//
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |  Opcode   |    Ra   |                   Disp                  |
//   +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
//   Memory Instruction Format
//
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |  Opcode   |    Ra   |    Rb   |            Disp               |
//   +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |  Opcode   |    Ra   |    Rb   |           Function            |
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
//   |  Opcode   |    Ra   |    Rb   | SBZ |0|   Function  |    Rc   |
//   +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |  Opcode   |    Ra   |     Literate  |1|   Function  |    Rc   |
//   +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//
//   Floating-Point Operate Instruction Format
//
//   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   |  Opcode   |    Fa   |    Fb   |       Function      |    Fc   |
//   +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//    3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//    1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

// Instruction Format Types
#define OP_FLOAT 0x0040 // 1 = Floating, 0 = Integer
#define OP_JUMP  0x0020 // With a Branch Displacement
#define OP_FUNC  0x0010 // With a Function Code
#define OP_TYPE  0x000F

#define OP_PCL   0 // PALcode Instruction Format
#define OP_BRA   1 // Branch Instruction Format
#define OP_MEM   2 // Memory Instruction Format
#define OP_OPR   3 // Operate Instruction Format

#define OP_MFC   (OP_MEM|OP_FUNC)
#define OP_MBR   (OP_MEM|OP_JUMP)
#define OP_FP    (OP_OPR|OP_FLOAT)

#define INST(name, opr, opc, fnc, flags) \
	{ name, opc, fnc, flags}

ALPHA_INST alpha_opCodes[] = {

//	INST("ADDF", "A,B,C", 0x15, 0x080, OP_FP),
//	INST("ADDG", "A,B,C", 0x15, 0x0A0, OP_FP),
//	INST("ADDL", "a,b,c", 0x10, 0x000, OP_OPR),
//	INST("ADDQ", "a,b,c", 0x10, 0x020, OP_OPR),
//	INST("ADDS", "A,B,C", 0x16, 0x080, OP_FP),
//	INST("ADDT", "A,B,C", 0x16, 0x0A0, OP_FP),

	{ "ADDF",     0x15, 0x080,  OP_FP },
	{ "ADDG",     0x15, 0x0A0,  OP_FP },
	{ "ADDL",     0x10, 0x000,  OP_OPR },
	{ "ADDQ",     0x10, 0x020,  OP_OPR },
	{ "ADDS",     0x16, 0x080,  OP_FP },
	{ "ADDT",     0x16, 0x0A0,  OP_FP },
	{ "AMASK",    0x11, 0x61,   OP_OPR},
	{ "AND",      0x11, 0x00,   OP_OPR},
	{ "BEQ",      0x39, 0x00,   OP_BRA},
	{ "BGE",      0x3E, 0x00,   OP_BRA},
	{ "BGT",      0x3F, 0x00,   OP_BRA},
	{ "BIC",      0x11, 0x08,   OP_OPR},
	{ "BIS",      0x11, 0x20,   OP_OPR},
	{ "BLBC",     0x38, 0x00,   OP_BRA},
	{ "BLBS",     0x3C, 0x00,   OP_BRA},
	{ "BLE",      0x3B, 0x00,   OP_BRA},
	{ "BLT",      0x3A, 0x00,   OP_BRA},
	{ "BNE",      0x3D, 0x00,   OP_BRA},
	{ "BR",       0x30, 0x00,   OP_BRA},
	{ "BSR",      0x34, 0x00,   OP_BRA},
	{ "CALL_PAL", 0x00, 0x00,   OP_PCL},
	{ "CMOVGE",   0x11, 0x46,   OP_OPR},
	{ "CMOVGT",   0x11, 0x66,   OP_OPR},
	{ "CMOVLBC",  0x11, 0x16,   OP_OPR},
	{ "CMOVLBS",  0x11, 0x14,   OP_OPR},
	{ "CMOVLE",   0x11, 0x64,   OP_OPR},
	{ "CMOVLT",   0x11, 0x44,   OP_OPR},
	{ "CMOVNE",   0x11, 0x26,   OP_OPR},
	{ "CMPBGE",   0x10, 0x0F,   OP_OPR},
	{ "CMPEQ",    0x10, 0x2D,   OP_OPR},
	{ "CMPGEQ",   0x15, 0x0A5,  OP_FP},
	{ "CMPGLE",   0x15, 0x0A7,  OP_FP},
	{ "CMPGLT",   0x15, 0x0A6,  OP_FP},
	{ "CMPLE",    0x10, 0x6D,   OP_OPR},
	{ "CMPLT",    0x10, 0x4D,   OP_OPR},
	{ "CMPTEQ",   0x16, 0x0A5,  OP_FP},
	{ "CMPTLE",   0x16, 0x0A7,  OP_FP},
	{ "CMPTLT",   0x16, 0x0A6,  OP_FP},
	{ "CMPTUN",   0x16, 0x0A4,  OP_FP},
	{ "CMPULE",   0x10, 0x3D,   OP_OPR},
	{ "CMPULT",   0x10, 0x1D,   OP_OPR},
	{ "CPYS",     0x17, 0x020,  OP_FP},
	{ "CPYSE",    0x17, 0x022,  OP_FP},
	{ "CPYSN",    0x17, 0x021,  OP_FP},
	{ "CTLZ",     0x1C, 0x32,   OP_OPR},
	{ "CTPOP",    0x1C, 0x30,   OP_OPR},
	{ "CTTZ",     0x1C, 0x33,   OP_OPR},
	{ "CVTDG",    0x15, 0x09E,  OP_FP},
	{ "CVTGD",    0x15, 0x0AD,  OP_FP},
	{ "CVTGF",    0x15, 0x0AC,  OP_FP},
	{ "CVTGQ",    0x15, 0x0AF,  OP_FP},
	{ "CVTLQ",    0x17, 0x010,  OP_FP},
	{ "CVTQF",    0x15, 0x0BC,  OP_FP},
	{ "CVTQG",    0x15, 0x0BE,  OP_FP},
	{ "CVTQL",    0x17, 0x030,  OP_FP},
	{ "CVTQS",    0x16, 0x0BC,  OP_FP},
	{ "CVTQT",    0x16, 0x0BE,  OP_FP},
	{ "CVTST",    0x16, 0x2AC,  OP_FP},
	{ "CVTTQ",    0x16, 0x0AF,  OP_FP},
	{ "CVTTS",    0x16, 0x0AC,  OP_FP},
	{ "DIVF",     0x15, 0x083,  OP_FP},
	{ "DIVG",     0x15, 0x0A3,  OP_FP},
	{ "DIVS",     0x16, 0x083,  OP_FP},
	{ "DIVT",     0x16, 0x0A3,  OP_FP},
	{ "ECB",      0x18, 0xE800, OP_MFC},
	{ "EQV",      0x11, 0x48,   OP_OPR},
	{ "EXCB",     0x18, 0x0400, OP_MFC},
	{ "EXTBL",    0x12, 0x06,   OP_OPR},
	{ "EXTLH",    0x12, 0x6A,   OP_OPR},
	{ "EXTLL",    0x12, 0x26,   OP_OPR},
	{ "EXTQH",    0x12, 0x7A,   OP_OPR},
	{ "EXTQL",    0x12, 0x36,   OP_OPR},
	{ "EXTWH",    0x12, 0x5A,   OP_OPR},
	{ "EXTWL",    0x12, 0x16,   OP_OPR},
	{ "FBEQ",     0x31, 0x00,   OP_BRA},
	{ "FBGE",     0x36, 0x00,   OP_BRA},
	{ "FBGT",     0x37, 0x00,   OP_BRA},
	{ "FBLE",     0x33, 0x00,   OP_BRA},
	{ "FBLT",     0x32, 0x00,   OP_BRA},
	{ "FBNE",     0x35, 0x00,   OP_BRA},
	{ "FCMOVEQ",  0x17, 0x02A,  OP_FP},
	{ "FCMOVGE",  0x17, 0x02D,  OP_FP},
	{ "FCMOVGT",  0x17, 0x02F,  OP_FP},
	{ "FCMOVLE",  0x17, 0x02E,  OP_FP},
	{ "FCMOVLT",  0x17, 0x02C,  OP_FP},
	{ "FCMOVNE",  0x17, 0x02B,  OP_FP},
	{ "FETCH",    0x18, 0x8000, OP_MFC},
	{ "FETCH_M",  0x18, 0xA000, OP_MFC},
	{ "FTOIS",    0x1C, 0x78,   OP_FP},
	{ "FTOIT",    0x1C, 0x70,   OP_FP},
	{ "IMPLVER",  0x11, 0x6C,   OP_OPR},
	{ "INSBL",    0x12, 0x0B,   OP_OPR},
	{ "INSLH",    0x12, 0x67,   OP_OPR},
	{ "INSLL",    0x12, 0x2B,   OP_OPR},
	{ "INSQH",    0x12, 0x77,   OP_OPR},
	{ "INSQL",    0x12, 0x3B,   OP_OPR},
	{ "INSWH",    0x12, 0x57,   OP_OPR},
	{ "INSWL",    0x12, 0x1B,   OP_OPR},
	{ "ITOFF",    0x14, 0x014,  OP_FP},
	{ "ITOFS",    0x14, 0x004,  OP_FP},
	{ "ITOFT",    0x14, 0x024,  OP_FP},
	{ "JMP",      0x1A, 0x0,    OP_MBR},
	{ "JSR",      0x1A, 0x1,    OP_MBR},
	{ "JSR_COROUTINE", 0x1A, 0x3, OP_MBR},
	{ "LDA",      0x08, 0,      OP_MEM},
	{ "LDAH",     0x09, 0,      OP_MEM},
	{ "LDBU",     0x0A, 0,      OP_MEM},
	{ "LDWU",     0x0C, 0,      OP_MEM},
	{ "LDF",      0x20, 0,      OP_MEM},
	{ "LDG",      0x21, 0,      OP_MEM},
	{ "LDL",      0x28, 0,      OP_MEM},
	{ "LDL_L",    0x2A, 0,      OP_MEM},
	{ "LDQ",      0x29, 0,      OP_MEM},
	{ "LDQ_L",    0x2B, 0,      OP_MEM},
	{ "LDQ_U",    0x0B, 0,      OP_MEM},
	{ "LDS",      0x22, 0,      OP_MEM},
	{ "LDT",      0x23, 0,      OP_MEM},
	{ "MAXSB8",   0x1C, 0x3E,   OP_OPR},
	{ "MAXSW4",   0x1C, 0x3F,   OP_OPR},
	{ "MAXUB8",   0x1C, 0x3C,   OP_OPR},
	{ "MAXUW4",   0x1C, 0x3D,   OP_OPR},
	{ "MB",       0x18, 0x4000, OP_MFC},
	{ "MF_FPCR",  0x17, 0x025,  OP_FP},
	{ "MINSB8",   0x1C, 0x38,   OP_OPR},
	{ "MINSW4",   0x1C, 0x39,   OP_OPR},
	{ "MINUB8",   0x1C, 0x3A,   OP_OPR},
	{ "MINUW4",   0x1C, 0x3B,   OP_OPR},
	{ "MSKBL",    0x12, 0x02,   OP_OPR},
	{ "MSKLH",    0x12, 0x62,   OP_OPR},
	{ "MSKLL",    0x12, 0x22,   OP_OPR},
	{ "MSKQH",    0x12, 0x72,   OP_OPR},
	{ "MSKQL",    0x12, 0x32,   OP_OPR},
	{ "MSKWH",    0x12, 0x52,   OP_OPR},
	{ "MSKWL",    0x12, 0x12,   OP_OPR},
	{ "MT_FPCR",  0x17, 0x024,  OP_FP},
	{ "MULF",     0x15, 0x082,  OP_FP},
	{ "MULG",     0x15, 0x0A2,  OP_FP},
	{ "MULL",     0x13, 0x00,   OP_OPR},
	{ "MULQ",     0x13, 0x20,   OP_OPR},
	{ "MULS",     0x16, 0x082,  OP_FP},
	{ "MULT",     0x16, 0x0A2,  OP_FP},
	{ "ORNOT",    0x11, 0x28,   OP_OPR},
	{ "PERR",     0x1C, 0x31,   OP_OPR},
	{ "PKLB",     0x1C, 0x37,   OP_OPR},
	{ "PKWB",     0x1C, 0x36,   OP_OPR},
	{ "RC",       0x18, 0xE000, OP_MFC},
	{ "RET",      0x1A, 0x2,    OP_MBR},
	{ "RPCC",     0x18, 0xC000, OP_MFC},
	{ "RS",       0x18, 0xF000, OP_MFC},
	{ "S4ADDL",   0x10, 0x02,   OP_OPR},
	{ "S4ADDQ",   0x10, 0x22,   OP_OPR},
	{ "S4SUBL",   0x10, 0x0B,   OP_OPR},
	{ "S4SUBQ",   0x10, 0x2B,   OP_OPR},
	{ "S8ADDL",   0x10, 0x12,   OP_OPR},
	{ "S8ADDQ",   0x10, 0x32,   OP_OPR},
	{ "S8SUBL",   0x10, 0x1B,   OP_OPR},
	{ "S8SUBQ",   0x10, 0x3B,   OP_OPR},
	{ "SEXTB",    0x1C, 0x00,   OP_OPR},
	{ "SEXTW",    0x1C, 0x01,   OP_OPR},
	{ "SLL",      0x12, 0x39,   OP_OPR},
	{ "SQRTF",    0x14, 0x08A,  OP_FP},
	{ "SQRTG",    0x14, 0x0AA,  OP_FP},
	{ "SQRTS",    0x14, 0x08B,  OP_FP},
	{ "SQRTT",    0x14, 0x0AB,  OP_FP},
	{ "SRA",      0x12, 0x3C,   OP_OPR},
	{ "SRL",      0x12, 0x34,   OP_OPR},
	{ "STB",      0x0E, 0,      OP_MEM},
	{ "STF",      0x24, 0,      OP_MEM},
	{ "STG",      0x25, 0,      OP_MEM},
	{ "STS",      0x26, 0,      OP_MEM},
	{ "STL",      0x2C, 0,      OP_MEM},
	{ "STL_C",    0x2E, 0,      OP_MEM},
	{ "STQ",      0x2D, 0,      OP_MEM},
	{ "STQ_C",    0x2F, 0,      OP_MEM},
	{ "STQ_U",    0x0F, 0,      OP_MEM},
	{ "STT",      0x27, 0,      OP_MEM},
	{ "STW",      0x0D, 0,      OP_MEM},
	{ "SUBF",     0x15, 0x081,  OP_FP},
	{ "SUBG",     0x15, 0x0A1,  OP_FP},
	{ "SUBL",     0x10, 0x09,   OP_OPR},
	{ "SUBQ",     0x10, 0x29,   OP_OPR},
	{ "SUBS",     0x16, 0x081,  OP_FP},
	{ "SUBT",     0x16, 0x0A1,  OP_FP},
	{ "TRAPB",    0x18, 0x0000, OP_MFC},
	{ "UMULH",    0x13, 0x30,   OP_OPR},
	{ "UNPKBL",   0x1C, 0x35,   OP_OPR},
	{ "UNPKBW",   0x1C, 0x34,   OP_OPR},
	{ "WH64",     0x18, 0xF800, OP_MFC},
	{ "WMB",      0x18, 0x4400, OP_MFC},
	{ "XOR",      0x11, 0x40,   OP_OPR},
	{ "ZAP",      0x12, 0x30,   OP_OPR},
	{ "ZAPNOT",   0x12, 0x31,   OP_OPR},

	{ "OPC01",    0x01, 0x00,   OP_PCL},
	{ "OPC02",    0x02, 0x00,   OP_PCL},
	{ "OPC03",    0x03, 0x00,   OP_PCL},
	{ "OPC04",    0x04, 0x00,   OP_PCL},
	{ "OPC05",    0x05, 0x00,   OP_PCL},
	{ "OPC06",    0x06, 0x00,   OP_PCL},
	{ "OPC07",    0x07, 0x00,   OP_PCL},

	{ "PAL19",    0x19, 0x00,   OP_PCL},
	{ "PAL1B",    0x1B, 0x00,   OP_PCL},
	{ "PAL1D",    0x1D, 0x00,   OP_PCL},
	{ "PAL1E",    0x1E, 0x00,   OP_PCL},
	{ "PAL1F",    0x1F, 0x00,   OP_PCL},

	// PAL instructions for 21164 processor.
	{ "HW_LD",    0x1B, 0x00,   OP_MEM},
	{ "HW_ST",    0x1F, 0x00,   OP_MEM},
	{ "HW_REI",   0x1E, 0x00,   OP_MEM},
	{ "HW_MFPR",  0x19, 0x00,   OP_MEM},
	{ "HW_MTPR",  0x1D, 0x00,   OP_MEM},

	{ NULL } // Null Terminator
	};

ALPHA_INST alpha_palCodes[] = {
#ifdef USE_PAL_VMS
	// OpenVMS Alpha Unprivileged PALcode Instructions
	{ "AMOVRM",       0x00, 0x00A1, OP_VMS},
	{ "AMOVRR",       0x00, 0x00A0, OP_VMS},
	{ "BPT",          0x00, 0x0080, OP_VMS},
	{ "BUGCHK",       0x00, 0x0081, OP_VMS},
	{ "CHMK",         0x00, 0x0083, OP_VMS},
	{ "CHME",         0x00, 0x0082, OP_VMS},
	{ "CHMS",         0x00, 0x0084, OP_VMS},
	{ "CHMU",         0x00, 0x0085, OP_VMS},
	{ "CLRFEN",       0x00, 0x00AE, OP_VMS},
	{ "GENTRAP",      0x00, 0x00AA, OP_VMS},
	{ "IMB",          0x00, 0x0086, OP_VMS},
	{ "INSQHIL",      0x00, 0x0087, OP_VMS},
	{ "INSQHILR",     0x00, 0x00A2, OP_VMS},
	{ "INSQHIQ",      0x00, 0x0089, OP_VMS},
	{ "INSQHIQR",     0x00, 0x00A4, OP_VMS},
	{ "INSQTIL",      0x00, 0x0088, OP_VMS},
	{ "INSQTILR",     0x00, 0x00A3, OP_VMS},
	{ "INSQTIQ",      0x00, 0x008A, OP_VMS},
	{ "INSQTIQR",     0x00, 0x00A5, OP_VMS},
	{ "INSQUEL",      0x00, 0x008B, OP_VMS},
	{ "INSQUEL/D",    0x00, 0x008D, OP_VMS},
	{ "INSQUEQ",      0x00, 0x008C, OP_VMS},
	{ "INSQUEQ/D",    0x00, 0x008E, OP_VMS},
	{ "PROBER",       0x00, 0x008F, OP_VMS},
	{ "PROBEW",       0x00, 0x0090, OP_VMS},
	{ "RD_PS",        0x00, 0x0091, OP_VMS},
	{ "READ_UNQ",     0x00, 0x009E, OP_VMS},
	{ "REI",          0x00, 0x0092, OP_VMS},
	{ "REMQHIL",      0x00, 0x0093, OP_VMS},
	{ "REMQHILR",     0x00, 0x00A8, OP_VMS},
	{ "REMQTIL",      0x00, 0x0094, OP_VMS},
	{ "REMQTILR",     0x00, 0x00A7, OP_VMS},
	{ "REMQTIQ",      0x00, 0x0096, OP_VMS},
	{ "REMQTIQR",     0x00, 0x00A9, OP_VMS},
	{ "REMQUEL",      0x00, 0x0097, OP_VMS},
	{ "REMQUEL/D",    0x00, 0x0099, OP_VMS},
	{ "REMQUEQ",      0x00, 0x0098, OP_VMS},
	{ "REMQUEQ/D",    0x00, 0x009A, OP_VMS},
	{ "RSCC",         0x00, 0x009D, OP_VMS},
	{ "SWASTEN",      0x00, 0x009B, OP_VMS},
	{ "WRITE_UNQ",    0x00, 0x009F, OP_VMS},
	{ "WR_PS_SW",     0x00, 0x009C, OP_VMS},

	// OpenVMS Alpha Privileged PALcode Instructions
	{ "CFLUSH",       0x00, 0x0001, OP_VMS},
	{ "CSERVE",       0x00, 0x0009, OP_VMS},
	{ "DRAINA",       0x00, 0x0002, OP_VMS},
	{ "HALT",         0x00, 0x0000, OP_VMS},
	{ "LDQP",         0x00, 0x0003, OP_VMS},
	{ "MFPR_ASN",     0x00, 0x0006, OP_VMS},
	{ "MPFR_ESP",     0x00, 0x001E, OP_VMS},
	{ "MPFR_FEN",     0x00, 0x000B, OP_VMS},
	{ "MPFR_IPL",     0x00, 0x000E, OP_VMS},
	{ "MPFR_MCES",    0x00, 0x0010, OP_VMS},
	{ "MPFR_PCBB",    0x00, 0x0012, OP_VMS},
	{ "MPFR_PRBR",    0x00, 0x0013, OP_VMS},
	{ "MPFR_PTBR",    0x00, 0x0015, OP_VMS},
	{ "MPFR_SCBB",    0x00, 0x0016, OP_VMS},
	{ "MPFR_SISR",    0x00, 0x0019, OP_VMS},
	{ "MPFR_SSP",     0x00, 0x0020, OP_VMS},
	{ "MPFR_TBCHK",   0x00, 0x001A, OP_VMS},
	{ "MPFR_USP",     0x00, 0x0022, OP_VMS},
	{ "MPFR_VPTB",    0x00, 0x0029, OP_VMS},
	{ "MPFR_WHAMI",   0x00, 0x003F, OP_VMS},
	{ "MTPR_ASTEN",   0x00, 0x0026, OP_VMS},
	{ "MTPR_ASTSR",   0x00, 0x0027, OP_VMS},
	{ "MTPR_DATFX",   0x00, 0x002E, OP_VMS},
	{ "MTPR_ESP",     0x00, 0x001F, OP_VMS},
	{ "MTPR_FEN",     0x00, 0x000C, OP_VMS},
	{ "MTPR_IPIR",    0x00, 0x000D, OP_VMS},
	{ "MTPR_IPL",     0x00, 0x000F, OP_VMS},
	{ "MTPR_MCES",    0x00, 0x0011, OP_VMS},
	{ "MTPR_PERFMON", 0x00, 0x002B, OP_VMS},
	{ "MTPR_PRBR",    0x00, 0x0014, OP_VMS},
	{ "MTPR_SCBB",    0x00, 0x0017, OP_VMS},
	{ "MTPR_SISR",    0x00, 0x0018, OP_VMS},
	{ "MTPR_SSP",     0x00, 0x0021, OP_VMS},
	{ "MTPR_TBIA",    0x00, 0x001B, OP_VMS},
	{ "MTPR_TBIAP",   0x00, 0x001C, OP_VMS},
	{ "MTPR_TBIS",    0x00, 0x001D, OP_VMS},
	{ "MTPR_TBISD",   0x00, 0x0024, OP_VMS},
	{ "MTPR_TBISI",   0x00, 0x0025, OP_VMS},
	{ "MTPR_USP",     0x00, 0x0023, OP_VMS},
	{ "MTPR_VPTB",    0x00, 0x002A, OP_VMS},
	{ "MTPR_WHAMI",   0x00, 0x003F, OP_VMS},
	{ "STQP",         0x00, 0x0004, OP_VMS},
	{ "SWPCTX",       0x00, 0x0005, OP_VMS},
	{ "SWPPAL",       0x00, 0x000A, OP_VMS},
	{ "WTINT",        0x00, 0x003E, OP_VMS},
#endif /* USE_PAL_VMS */

#ifdef USE_PAL_UNIX
	// DIGITAL UNIX Unprivleged PALcode Instructions
	{ "BPT",          0x00, 0x0080, OP_UNIX},
	{ "BUGCHK",       0x00, 0x0081, OP_UNIX},
	{ "CALLSYS",      0x00, 0x0083, OP_UNIX},
	{ "CLRFEN",       0x00, 0x00AE, OP_UNIX},
	{ "GENTRAP",      0x00, 0x00AA, OP_UNIX},
	{ "IMB",          0x00, 0x0086, OP_UNIX},
	{ "RDUNIQUE",     0x00, 0x009E, OP_UNIX},
	{ "URTI",         0x00, 0x0092, OP_UNIX},
	{ "WRUNIQUE",     0x00, 0x009F, OP_UNIX},

	// DIGITAL UNIX Privileged PAL Instructions
	{ "CFLUSH",       0x00, 0x0001, OP_UNIX},
	{ "CSERVE",       0x00, 0x0009, OP_UNIX},
	{ "DRAINA",       0x00, 0x0002, OP_UNIX},
	{ "HALT",         0x00, 0x0000, OP_UNIX},
	{ "RDMCES",       0x00, 0x0010, OP_UNIX},
	{ "RDPS",         0x00, 0x0036, OP_UNIX},
	{ "RDUSP",        0x00, 0x003A, OP_UNIX},
	{ "RDVAL",        0x00, 0x0032, OP_UNIX},
	{ "RETSYS",       0x00, 0x003D, OP_UNIX},
	{ "RTI",          0x00, 0x003F, OP_UNIX},
	{ "SWPCTX",       0x00, 0x0030, OP_UNIX},
	{ "SWPIPL",       0x00, 0x0035, OP_UNIX},
	{ "SWPPAL",       0x00, 0x000A, OP_UNIX},
	{ "TBI",          0x00, 0x0033, OP_UNIX},
	{ "WHAMI",        0x00, 0x003C, OP_UNIX},
	{ "WRENT",        0x00, 0x0034, OP_UNIX},
	{ "WRFEN",        0x00, 0x002B, OP_UNIX},
	{ "WRIPIR",       0x00, 0x000D, OP_UNIX},
	{ "WRKGP",        0x00, 0x0037, OP_UNIX},
	{ "WRMCES",       0x00, 0x0011, OP_UNIX},
	{ "WRPERFMON",    0x00, 0x0039, OP_UNIX},
	{ "WRUSP",        0x00, 0x0038, OP_UNIX},
	{ "WRVAL",        0x00, 0x0031, OP_UNIX},
	{ "WRVPTPTR",     0x00, 0x002D, OP_UNIX},
	{ "WTINT",        0x00, 0x003E, OP_UNIX},
#endif /* USE_PAL_UNIX */

#ifdef USE_PAL_WNT
	// Windows NT Unprivleged PALcode Instructions
	{ "BPT",          0x00, 0x0080, OP_WNT},
	{ "CALLKD",       0x00, 0x00AD, OP_WNT},
	{ "CALLSYS",      0x00, 0x0083, OP_WNT},
	{ "GENTRAP",      0x00, 0x00AA, OP_WNT},
	{ "IMB",          0x00, 0x0086, OP_WNT},
	{ "KBPT",         0x00, 0x00AC, OP_WNT},
	{ "RDTEB",        0x00, 0x00AB, OP_WNT},

	// Windows NT Privileged PALcode Instructions
	{ "CSIR",         0x00, 0x000D, OP_WNT},
	{ "DALNFIX",      0x00, 0x0025, OP_WNT},
	{ "DI",           0x00, 0x0008, OP_WNT},
	{ "DRAINA",       0x00, 0x0002, OP_WNT},
	{ "DTBIS",        0x00, 0x0016, OP_WNT},
	{ "EALNFIX",      0x00, 0x0024, OP_WNT},
	{ "EI",           0x00, 0x0009, OP_WNT},
	{ "HALT",         0x00, 0x0000, OP_WNT},
	{ "INITPAL",      0x00, 0x0004, OP_WNT},
	{ "INITPCR",      0x00, 0x0038, OP_WNT},
	{ "RDCOUNTERS",   0x00, 0x0030, OP_WNT},
	{ "RDIRQL",       0x00, 0x0007, OP_WNT},
	{ "RDKSP",        0x00, 0x0018, OP_WNT},
	{ "RDMCES",       0x00, 0x0012, OP_WNT},
	{ "RDPCR",        0x00, 0x001C, OP_WNT},
	{ "RDPSR",        0x00, 0x001A, OP_WNT},
	{ "RDSTATE",      0x00, 0x0031, OP_WNT},
	{ "RDTHREAD",     0x00, 0x001E, OP_WNT},
	{ "REBOOT",       0x00, 0x0002, OP_WNT},
	{ "RESTART",      0x00, 0x0001, OP_WNT},
	{ "RETSYS",       0x00, 0x000F, OP_WNT},
	{ "RFE",          0x00, 0x000E, OP_WNT},
	{ "SWPIRQL",      0x00, 0x0006, OP_WNT},
	{ "SWPKSP",       0x00, 0x0019, OP_WNT},
	{ "SWPPAL",       0x00, 0x000A, OP_WNT},
	{ "SWPPROCESS",   0x00, 0x0011, OP_WNT},
	{ "SWPCTX",       0x00, 0x0010, OP_WNT},
	{ "SSIR",         0x00, 0x000C, OP_WNT},
	{ "TBIA",         0x00, 0x0014, OP_WNT},
	{ "TBIM",         0x00, 0x0020, OP_WNT},
	{ "TBIMASN",      0x00, 0x0021, OP_WNT},
	{ "TBIS",         0x00, 0x0015, OP_WNT},
	{ "TBISASN",      0x00, 0x0017, OP_WNT},
	{ "WRENTRY",      0x00, 0x0005, OP_WNT},
	{ "WRMCES",       0x00, 0x0013, OP_WNT},
	{ "WRPERFMON",    0x00, 0x0032, OP_WNT},
#endif /* USE_PAL_WNT */

	{ NULL } // Null Terminator
};
#endif
