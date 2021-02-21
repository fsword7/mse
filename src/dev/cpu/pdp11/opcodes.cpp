/*
 * opcodes.cpp
 *
 *  Created on: Apr 22, 2017
 *      Author: Tim Stark
 *
 *  PDP-11 instruction set
 *
 */

// List of PDP-11 Instruction
//
// SINGLE-OPERAND INSTRUCTION
// --------------------------
//
// General
//
//   Mnemonic Instruction                Op Code
//   -------- -----------                -------
//   CLR(B)   Clear Destination          B050DD
//   COM(B)   Complement Destination     B051DD
//   INC(B)   Increment Destination      B052DD
//   DEC(B)   Decrement Destination      B053DD
//   NEG(B)   Negate Destination         B054DD
//   TST(B)   Test Destination           B057DD
//
// Shift and Rotate
//
//   Mnemonic Instruction                Op Code
//   -------- -----------                -------
//   ASR(B)   Arithmetic Shift Right     B062DD
//   ASL(B)   Arithmetic Shift Left      B063DD
//   ROR(B)   Rotate Right               B060DD
//   ROL(B)   Rotate Left                B061DD
//   SWAB     Swap Bytes                 0003DD
//
// Multiple-Precision
//
//   Mnemonic Instruction                Op Code
//   -------- -----------                -------
//   ADC(B)   Add Carry                  B055DD
//   SBC(B)   Subtract Carry             B056DD
//   SXT      Sign Extend                0067DD
//
// Processor Status Word Operators
//
//   Mnemonic Instruction                Op Code
//   -------- -----------                -------
//   MFPS     Move Byte from PS          1067DD
//   MTPS     Move Byte to PS            1064SS
//
// DOUBLE-OPERAND INSTRUCTION
// --------------------------
//
// General
//
//   Mnemonic Instruction                    Op Code
//   -------- -----------                    -------
//   MOV(B)   Move Source to Destination     B1SSDD
//   CMP(B)   Compare Source to Destination  B2SSDD
//   ADD      Add Source to Destination      06SSDD
//   SUB      Subtract Source to Destination 16SSDD
//
// Logical
//
//   Mnemonic Instruction                    Op Code
//   -------- -----------                    -------
//   BIT(B)   Bit Test                       B3SSDD
//   BIC(B)   Bit Clear                      B4SSDD
//   BIS(B)   Bit Set                        B5SSDD
//   XOR      Exclusive OR                   074RDD
//
// PROGRAM CONTROL INSTRUCTION
// ---------------------------
//
// Branch
//
//   Mnemonic Instruction                    Op Code
//   -------- -----------                    -------
//   BR       Branch (Unconditional)         000400
//   BNE      Branch if Not Equal            001000
//   BEQ      Branch if Equal                001400
//   BPL      Branch if Plus                 100000
//   BMI      Branch if Minus                100400
//   BVC      Branch if Overflow Clear       102000
//   BVS      Branch if Overflow Set         102400
//   BCC      Branch if Carry Clear          103000
//   BCS      Branch if Carry Set            103400
//
// Signed Conditional Branch
//
//   Mnemonic Instruction                     Op Code
//   -------- -----------                     -------
//   BGE      Branch if Greater Than or Equal 002000
//   BLT      Branch if Less Than             002400
//   BGT      Branch if Greater Than          003000
//   BLE      Branch if Less Than or Equal    003400
//
// Unsigned Conditional Branch
//
//   Mnemonic Instruction                     Op Code
//   -------- -----------                     -------
//   BHI      Branch if Higher                101000
//   BLOS     Branch if Lower or Same         101400
//   BHIS     Branch if Higher or Same        103000
//   BLO      Branch if Lower                 103400
//
// Jump and Subroutine
//
//   Mnemonic Instruction                     Op Code
//   -------- -----------                     -------
//   JMP      Jump                            101000
//   JSR      Jump to Subroutine              004RDD
//   RTS      Return from Subroutine          00020R
//   SOB      Subtract One and Branch         077R00
//
// Trap and Interrupt
//
//   Mnemonic Instruction                     Op Code
//   -------- -----------                     -------
//   EMT      Emulator Trap                   104000-104377
//   TRAP     Trap                            104400-104777
//   BPT      Breakpoint Trap                 000003
//   IOT      Input/Output Trap               000004
//   RTI      Return from Interrupt           000002
//   RTT      Return from Interrupt           000006
//
// MISCELLANEOUS INSTRUCTION
// -------------------------
//
//   Mnemonic Instruction                     Op Code
//   -------- -----------                     -------
//   HALT     Halt                            000000
//   WAIT     Wait                            000001
//   RESET    Reset External Bus              000005
//   MFPT     Move Processor Type             000007
//
// CONDITION CODE OPERATORS
// ------------------------
//
//   Mnemonic Instruction                     Op Code
//   -------- -----------                     -------
//   CLC      Clear C (Carry)                 000241
//   CLV      Clear V (Overflow)              000242
//   CLZ      Clear Z (Zero)                  000244
//   CLN      Clear N (Negate)                000250
//   CCC      Clear All CC Bits               000257
//   SEC      Set C (Carry)                   000261
//   SEV      Set V (Overflow)                000262
//   SEZ      Set Z (Zero)                    000264
//   SEN      Set N (Negate)                  000270
//   SCC      Set All CC Bits                 000277
//   NOP      No Operation                    000240

// INSTRUCTION FORMATS
// -------------------
//
// Single Operand Instruction
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |         Opcode              |     Src/Dst     |
// +--^--+--+--^--+--+--^--+--+--^--+--+--^--+--+--+
//  15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
//
// Double Operand Instruction
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |  Opcode   |       Src       |       Dst       |
// +--^--+--+--^--+--+--^--+--+--^--+--+--^--+--+--+
//  15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
//
// Branch Instruction
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |       Opcode          |        Offset         |
// +--^--+--+--^--+--+--^--+--+--^--+--+--^--+--+--+
//  15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
//
// Jump to Subroutine (JSR) Instruction
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |         004        |   Reg  |       Dst       |
// +--^--+--+--^--+--+--^--+--+--^--+--+--^--+--+--+
//  15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
//
// Operate Instruction
//
// HALT, MFPT, RESET, RIT, RTT, and WAIT
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |                      Opcode                   |
// +--^--+--+--^--+--+--^--+--+--^--+--+--^--+--+--+
//  15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
//
// Condition Code Operator Instruction
//
// NOP, CCC, CLC, CLN, CLV, CLZ, SCC, SEC, SEN, SEV, and SEZ
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// |              00024             |S |N |Z |V |C |
// +--^--+--+--^--+--+--^--+--+--^--+--+--^--+--+--+
//  15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
//
// S = (Set = 1, Clear = 0)
// N = Negate Bit
// Z = Zero Bit
// V = Overflow Bit
// C = Carry Bit

#if 0
#include "main/defs.h"
#include "cpu/pdp11/defs.h"

// PDP-11 Instruction Table
p11opc_t p11_Opcodes[] =
{
	// Basic Instructions
	{ "ADCB",   OP_SOP,     0,        0, 0105500, 0000077, DEF_NAME(p11cpu, ADCB) },
	{ "ADC",    OP_SOP,     0,        0, 0005500, 0000077, DEF_NAME(p11cpu, ADC)   },
	{ "ADD",    OP_DOP,     0,        0, 0060000, 0007777, DEF_NAME(p11cpu, ADD)   },
	{ "ASLB",   OP_SOP,     0,        0, 0106300, 0000077, DEF_NAME(p11cpu, ASLB)  },
	{ "ASL",    OP_SOP,     0,        0, 0006300, 0000077, DEF_NAME(p11cpu, ASL)   },
	{ "ASRB",   OP_SOP,     0,        0, 0106200, 0000077, DEF_NAME(p11cpu, ASRB)  },
	{ "ASR",    OP_SOP,     0,        0, 0006200, 0000077, DEF_NAME(p11cpu, ASR)  },
	{ "BCC",    OP_BR,      0,        0, 0103000, 0000377, DEF_NAME(p11cpu, BCC)  },
	{ "BCS",    OP_BR,      0,        0, 0103400, 0000377, DEF_NAME(p11cpu, BCS)  },
	{ "BEQ",    OP_BR,      0,        0, 0001400, 0000377, DEF_NAME(p11cpu, BEQ)  },
	{ "BGE",    OP_BR,      0,        0, 0002000, 0000377, DEF_NAME(p11cpu, BGE)  },
	{ "BGT",    OP_BR,      0,        0, 0003000, 0000377, DEF_NAME(p11cpu, BGT)  },
	{ "BHI",    OP_BR,      0,        0, 0101000, 0000377, DEF_NAME(p11cpu, BHI)  },
	{ "BHIS",   OP_BR,      0,        0, 0103000, 0000377, DEF_NAME(p11cpu, BCC) },
	{ "BICB",   OP_DOP,     0,        0, 0140000, 0007777, DEF_NAME(p11cpu, BICB)  },
	{ "BIC",    OP_DOP,     0,        0, 0040000, 0007777, DEF_NAME(p11cpu, BIC)  },
	{ "BISB",   OP_DOP,     0,        0, 0150000, 0007777, DEF_NAME(p11cpu, BISB)  },
	{ "BIS",    OP_DOP,     0,        0, 0050000, 0007777, DEF_NAME(p11cpu, BIS)  },
	{ "BITB",   OP_DOP,     0,        0, 0130000, 0007777, DEF_NAME(p11cpu, BITB)  },
	{ "BIT",    OP_DOP,     0,        0, 0030000, 0007777, DEF_NAME(p11cpu, BIT)  },
	{ "BLE",    OP_BR,      0,        0, 0003400, 0000377, DEF_NAME(p11cpu, BLE)  },
	{ "BLO",    OP_BR,      0,        0, 0103400, 0000377, DEF_NAME(p11cpu, BCS)  },
	{ "BLOS",   OP_BR,      0,        0, 0101400, 0000377, DEF_NAME(p11cpu, BLOS)  },
	{ "BLT",    OP_BR,      0,        0, 0002400, 0000377, DEF_NAME(p11cpu, BLT)  },
	{ "BMI",    OP_BR,      0,        0, 0100400, 0000377, DEF_NAME(p11cpu, BMI)  },
	{ "BNE",    OP_BR,      0,        0, 0001000, 0000377, DEF_NAME(p11cpu, BNE)  },
	{ "BPL",    OP_BR,      0,        0, 0100000, 0000377, DEF_NAME(p11cpu, BPL)  },
	{ "BPT",    OP_NPN,     0,        0, 0000003, 0000000, DEF_NAME(p11cpu, BPT)  },
	{ "BR",     OP_BR,      0,        0, 0000400, 0000377, DEF_NAME(p11cpu, BR)  },
	{ "BVC",    OP_BR,      0,        0, 0102000, 0000377, DEF_NAME(p11cpu, BVC) },
	{ "BVS",    OP_BR,      0,        0, 0102400, 0000377, DEF_NAME(p11cpu, BVS) },
	{ "CLRB",   OP_SOP,     0,        0, 0105000, 0000077, DEF_NAME(p11cpu, CLRB)  },
	{ "CLR",    OP_SOP,     0,        0, 0005000, 0000077, DEF_NAME(p11cpu, CLR)   },
	{ "CMPB",   OP_DOP,     0,        0, 0120000, 0007777, DEF_NAME(p11cpu, CMPB)  },
	{ "CMP",    OP_DOP,     0,        0, 0020000, 0007777, DEF_NAME(p11cpu, CMP)   },
	{ "COMB",   OP_SOP,     0,        0, 0105100, 0000077, DEF_NAME(p11cpu, COMB)  },
	{ "COM",    OP_SOP,     0,        0, 0005100, 0000077, DEF_NAME(p11cpu, COM)  },
	{ "DECB",   OP_SOP,     0,        0, 0105300, 0000077, DEF_NAME(p11cpu, DECB)  },
	{ "DEC",    OP_SOP,     0,        0, 0005300, 0000077, DEF_NAME(p11cpu, DEC)   },
	{ "EMT",    OP_8BIT,    0,        0, 0104000, 0000377, DEF_NAME(p11cpu, EMT)  },
	{ "HALT",   OP_NPN,     0,        0, 0000000, 0000000, DEF_NAME(p11cpu, HALT) },
	{ "INCB",   OP_SOP,     0,        0, 0105200, 0000077, DEF_NAME(p11cpu, INCB)  },
	{ "INC",    OP_SOP,     0,        0, 0005200, 0000077, DEF_NAME(p11cpu, INC)   },
	{ "IOT",    OP_NPN,     0,        0, 0000004, 0000000, DEF_NAME(p11cpu, IOT)  },
	{ "JMP",    OP_SOP,     0,        0, 0000100, 0000077, DEF_NAME(p11cpu, JMP)  },
	{ "JSR",    OP_RSOP,    0,        0, 0004000, 0000777, DEF_NAME(p11cpu, JSR)  },
	{ "MOVB",   OP_DOP,     0,        0, 0110000, 0007777, DEF_NAME(p11cpu, MOVB)  },
	{ "MOV",    OP_DOP,     0,        0, 0010000, 0007777, DEF_NAME(p11cpu, MOV)  },
	{ "NEGB",   OP_SOP,     0,        0, 0105400, 0000077, DEF_NAME(p11cpu, NEGB)  },
	{ "NEG",    OP_SOP,     0,        0, 0005400, 0000077, DEF_NAME(p11cpu, NEG)   },
	{ "RESET",  OP_NPN,     0,        0, 0000005, 0000000, DEF_NAME(p11cpu, RESET)  },
	{ "ROLB",   OP_SOP,     0,        0, 0106100, 0000077, DEF_NAME(p11cpu, ROLB)  },
	{ "ROL",    OP_SOP,     0,        0, 0006100, 0000077, DEF_NAME(p11cpu, ROL)  },
	{ "RORB",   OP_SOP,     0,        0, 0106000, 0000077, DEF_NAME(p11cpu, RORB)  },
	{ "ROR",    OP_SOP,     0,        0, 0006000, 0000077, DEF_NAME(p11cpu, ROR)   },
	{ "RTI",    OP_NPN,     0,        0, 0000002, 0000000, DEF_NAME(p11cpu, RTI)  },
	{ "RTS",    OP_REG,     0,        0, 0000200, 0000007, DEF_NAME(p11cpu, RTS)  },
	{ "SBCB",   OP_SOP,     0,        0, 0105600, 0000077, DEF_NAME(p11cpu, SBCB) },
	{ "SBC",    OP_SOP,     0,        0, 0005600, 0000077, DEF_NAME(p11cpu, SBC)  },
	{ "SUB",    OP_DOP,     0,        0, 0160000, 0007777, DEF_NAME(p11cpu, SUB)  },
	{ "SWAB",   OP_SOP,     0,        0, 0000300, 0000077, DEF_NAME(p11cpu, SWAB) },
	{ "TRAP",   OP_8BIT,    0,        0, 0104400, 0000377, DEF_NAME(p11cpu, TRAP)  } ,
	{ "TSTB",   OP_SOP,     0,        0, 0105700, 0000077, DEF_NAME(p11cpu, TSTB) },
	{ "TST",    OP_SOP,     0,        0, 0005700, 0000077, DEF_NAME(p11cpu, TST)  },
	{ "WAIT",   OP_NPN,     0,        0, 0000001, 0000000, DEF_NAME(p11cpu, WAIT)  },
	{ "Cxx",    OP_CCC,     0,        0, 0000240, 0000017, DEF_NAME(p11cpu, Xcc)  },
	{ "NOP",    OP_CCC,     0,        0, 0000240, 0000000, DEF_NAME(p11cpu, Xcc)  },
	{ "CLC",    OP_CCC,     0,        0, 0000241, 0000000, DEF_NAME(p11cpu, Xcc)  },
	{ "CLV",    OP_CCC,     0,        0, 0000242, 0000000, DEF_NAME(p11cpu, Xcc)  },
	{ "CLZ",    OP_CCC,     0,        0, 0000244, 0000000, DEF_NAME(p11cpu, Xcc)  },
	{ "CLN",    OP_CCC,     0,        0, 0000250, 0000000, DEF_NAME(p11cpu, Xcc)  },
	{ "CCC",    OP_CCC,     0,        0, 0000257, 0000000, DEF_NAME(p11cpu, Xcc)  },
	{ "Sxx",    OP_SCC,     0,        0, 0000260, 0000017, DEF_NAME(p11cpu, Xcc)  },
	{ "NOP",    OP_SCC,     0,        0, 0000260, 0000000, DEF_NAME(p11cpu, Xcc)  },
	{ "SEC",    OP_SCC,     0,        0, 0000261, 0000000, DEF_NAME(p11cpu, Xcc)  },
	{ "SEV",    OP_SCC,     0,        0, 0000262, 0000000, DEF_NAME(p11cpu, Xcc)  },
	{ "SEZ",    OP_SCC,     0,        0, 0000264, 0000000, DEF_NAME(p11cpu, Xcc)  },
	{ "SEN",    OP_SCC,     0,        0, 0000270, 0000000, DEF_NAME(p11cpu, Xcc)  },
	{ "SCC",    OP_SCC,     0,        0, 0000277, 0000000, DEF_NAME(p11cpu, Xcc)  },

	// Model-Specific Instruction Set
	{ "MFPT",   OP_NPN,     OPT_MFPT, 0, 0000007, 0000000, DEF_NAME(p11cpu, MFPT) },

	{ "MTPS",   OP_SOP,     OPT_MxPS, 0, 0106400, 0000077, DEF_NAME(p11cpu, MTPS) },
	{ "MFPS",   OP_SOP,     OPT_MxPS, 0, 0106700, 0000077, DEF_NAME(p11cpu, MFPS) },

	{ "MFPD",   OP_SOP,     OPT_MxPy, 0, 0106500, 0000077, DEF_NAME(p11cpu, MFPD) },
	{ "MFPI",   OP_SOP,     OPT_MxPy, 0, 0006500, 0000077, DEF_NAME(p11cpu, MFPI) } ,
	{ "MTPD",   OP_SOP,     OPT_MxPy, 0, 0106600, 0000077, DEF_NAME(p11cpu, MTPD) },
	{ "MTPI",   OP_SOP,     OPT_MxPy, 0, 0006600, 0000077, DEF_NAME(p11cpu, MTPI) },
	{ "CSM",    OP_SOP,     OPT_CSM,  0, 0007100, 0000077, DEF_NAME(p11cpu, CSM)  },
	{ "SPL",    OP_3BIT,    OPT_SPL,  0, 0000230, 0000007, DEF_NAME(p11cpu, SPL)  },

	{ "SOB",    OP_SOB,     OPT_SXS,  0, 0077000, 0000777, DEF_NAME(p11cpu, SOB)  },
	{ "SXT",    OP_SOP,     OPT_SXS,  0, 0006700, 0000077, DEF_NAME(p11cpu, SXT)  },
	{ "XOR",    OP_RSOP,    OPT_SXS,  0, 0074000, 0000777, DEF_NAME(p11cpu, XOR)  },
	{ "MARK",   OP_SOP,     OPT_MARK, 0, 0006400, 0000077, DEF_NAME(p11cpu, MARK) },

	{ "RTT",    OP_NPN,     OPT_RTT,  0, 0000006, 0000000, DEF_NAME(p11cpu, RTT)  },
	{ "TSTSET", OP_SOP,     OPT_TSWLK, 0, 0007200, 0000077, DEF_NAME(p11cpu, TSTSET) },
	{ "WRTLCK", OP_SOP,     OPT_TSWLK, 0, 0007300, 0000077, DEF_NAME(p11cpu, WRTLCK)  },

	// Enhanced Instruction Set
	{ "ASHC",   OP_RSOP,    0, OPT_EIS,  0073000, 0000777, DEF_NAME(p11eis, ASHC) },
	{ "ASH",    OP_RSOP,    0, OPT_EIS,  0072000, 0000777, DEF_NAME(p11eis, ASH)  },
	{ "DIV",    OP_RSOP,    0, OPT_EIS,  0071000, 0000777, DEF_NAME(p11eis, DIV)  },
	{ "MUL",    OP_RSOP,    0, OPT_EIS,  0070000, 0000777, DEF_NAME(p11eis, MUL)  },

	// Floating Instruction Set
	{ "FADD",   OP_REG,     0, OPT_FIS,  0075000, 0000007, NULL   },
	{ "FDIV",   OP_REG,     0, OPT_FIS,  0075030, 0000007, NULL   },
	{ "FMUL",   OP_REG,     0, OPT_FIS,  0075020, 0000007, NULL   },
	{ "FSUB",   OP_REG,     0, OPT_FIS,  0075010, 0000007, NULL   },

	// Complex Instruction Set
	{ "LD2R",   OP_REG,     0, OPT_CIS,  0076020, 0000007, DEF_NAME(p11cis, LDxR)   },
	{ "LD3R",   OP_REG,     0, OPT_CIS,  0076060, 0000007, DEF_NAME(p11cis, LDxR)   },

	{ "CMPCI",  OP_NPN,     0, OPT_CIS,  0076144, 0000000, NULL   },
	{ "CMPC",   OP_NPN,     0, OPT_CIS,  0076044, 0000000, NULL   },
	{ "LOCCI",  OP_NPN,     0, OPT_CIS,  0076140, 0000000, NULL   },
	{ "LOCC",   OP_NPN,     0, OPT_CIS,  0076040, 0000000, NULL   },
	{ "MATCI",  OP_NPN,     0, OPT_CIS,  0076145, 0000000, NULL   },
	{ "MATC",   OP_NPN,     0, OPT_CIS,  0076045, 0000000, NULL   },
	{ "MOVCI",  OP_NPN,     0, OPT_CIS,  0076130, 0000000, NULL   },
	{ "MOVC",   OP_NPN,     0, OPT_CIS,  0076030, 0000000, NULL   },
	{ "MOVRCI", OP_NPN,     0, OPT_CIS,  0076131, 0000000, NULL   },
	{ "MOVRC",  OP_NPN,     0, OPT_CIS,  0076031, 0000000, NULL   },
	{ "MOVTCI", OP_NPN,     0, OPT_CIS,  0076132, 0000000, NULL   },
	{ "MOVTC",  OP_NPN,     0, OPT_CIS,  0076032, 0000000, NULL   },
	{ "SCANCI", OP_NPN,     0, OPT_CIS,  0076142, 0000000, NULL   },
	{ "SCANC",  OP_NPN,     0, OPT_CIS,  0076042, 0000000, NULL   },
	{ "SKPCI",  OP_NPN,     0, OPT_CIS,  0076141, 0000000, NULL   },
	{ "SKPC",   OP_NPN,     0, OPT_CIS,  0076041, 0000000, NULL   },
	{ "SPANCI", OP_NPN,     0, OPT_CIS,  0076143, 0000000, NULL   },
	{ "SPANC",  OP_NPN,     0, OPT_CIS,  0076043, 0000000, NULL   },

	{ "ADDPI",  OP_NPN,     0, OPT_CIS,  0076170, 0000000, NULL   },
	{ "ADDP",   OP_NPN,     0, OPT_CIS,  0076070, 0000000, NULL   },
	{ "ASHPI",  OP_NPN,     0, OPT_CIS,  0076176, 0000000, NULL   },
	{ "ASHP",   OP_NPN,     0, OPT_CIS,  0076076, 0000000, NULL   },
	{ "CMPPI",  OP_NPN,     0, OPT_CIS,  0076172, 0000000, NULL   },
	{ "CMPP",   OP_NPN,     0, OPT_CIS,  0076072, 0000000, NULL   },
	{ "CVTLPI", OP_NPN,     0, OPT_CIS,  0076177, 0000000, NULL   },
	{ "CVTLP",  OP_NPN,     0, OPT_CIS,  0076077, 0000000, NULL   },
	{ "CVTPLI", OP_NPN,     0, OPT_CIS,  0076173, 0000000, NULL   },
	{ "CVTPL",  OP_NPN,     0, OPT_CIS,  0076073, 0000000, NULL   },
	{ "DIVPI",  OP_NPN,     0, OPT_CIS,  0076175, 0000000, NULL   },
	{ "DIVP",   OP_NPN,     0, OPT_CIS,  0076075, 0000000, NULL   },
	{ "MULPI",  OP_NPN,     0, OPT_CIS,  0076174, 0000000, NULL   },
	{ "MULP",   OP_NPN,     0, OPT_CIS,  0076074, 0000000, NULL   },
	{ "SUBPI",  OP_NPN,     0, OPT_CIS,  0076171, 0000000, NULL   },
	{ "SUBP",   OP_NPN,     0, OPT_CIS,  0076071, 0000000, NULL   },

	{ "ASHNI",  OP_NPN,     0, OPT_CIS,  0076156, 0000000, NULL   },
	{ "ASHN",   OP_NPN,     0, OPT_CIS,  0076056, 0000000, NULL   },
	{ "ADDNI",  OP_NPN,     0, OPT_CIS,  0076150, 0000000, NULL   },
	{ "ADDN",   OP_NPN,     0, OPT_CIS,  0076050, 0000000, NULL   },
	{ "CMPNI",  OP_NPN,     0, OPT_CIS,  0076152, 0000000, NULL   },
	{ "CMPN",   OP_NPN,     0, OPT_CIS,  0076052, 0000000, NULL   },
	{ "CVTLNI", OP_NPN,     0, OPT_CIS,  0076157, 0000000, NULL   },
	{ "CVTLN",  OP_NPN,     0, OPT_CIS,  0076057, 0000000, NULL   },
	{ "CVTNLI", OP_NPN,     0, OPT_CIS,  0076153, 0000000, NULL   },
	{ "CVTNL",  OP_NPN,     0, OPT_CIS,  0076053, 0000000, NULL   },
	{ "CVTNPI", OP_NPN,     0, OPT_CIS,  0076155, 0000000, NULL   },
	{ "CVTNP",  OP_NPN,     0, OPT_CIS,  0076055, 0000000, NULL   },
	{ "CVTPNI", OP_NPN,     0, OPT_CIS,  0076154, 0000000, NULL   },
	{ "CVTPN",  OP_NPN,     0, OPT_CIS,  0076054, 0000000, NULL   },
	{ "SUBNI",  OP_NPN,     0, OPT_CIS,  0076151, 0000000, NULL   },
	{ "SUBN",   OP_NPN,     0, OPT_CIS,  0076051, 0000000, NULL   },

	// Floating Instruction Set
	{ "ABSf",   OP_FOP,     0, OPT_FPP,  0170600, 0000077, NULL   },
//	{ "ABSD",   OP_FOP_D,   0, OPT_FPP,  0170600, 0000000, NULL   },
//	{ "ABSF",   OP_FOP_F,   0, OPT_FPP,  0170600, 0000000, NULL   },
	{ "ADDf",   OP_AFOP,    0, OPT_FPP,  0172000, 0000377, NULL   },
//	{ "ADDD",   OP_AFOP_D,  0, OPT_FPP,  0172000, 0000000, NULL   },
//	{ "ADDF",   OP_AFOP_F,  0, OPT_FPP,  0172000, 0000000, NULL   },
	{ "CFCC",   OP_NPN,     0, OPT_FPP,  0170000, 0000000, NULL   },
	{ "CLRf",   OP_FOP,     0, OPT_FPP,  0170400, 0000377, NULL   },
//	{ "CLRD",   OP_FOP_D,   0, OPT_FPP,  0170400, 0000000, NULL   },
//	{ "CLRF",   OP_FOP_F,   0, OPT_FPP,  0170400, 0000000, NULL   },
	{ "CMPf",   OP_AFOP,    0, OPT_FPP,  0173400, 0000377, NULL   },
//	{ "CMPD",   OP_AFOP_D,  0, OPT_FPP,  0173400, 0000000, NULL   },
//	{ "CMPF",   OP_AFOP_F,  0, OPT_FPP,  0173400, 0000000, NULL   },
	{ "DIVf",   OP_AFOP,    0, OPT_FPP,  0174400, 0000377, NULL   },
//	{ "DIVD",   OP_AFOP_D,  0, OPT_FPP,  0174400, 0000000, NULL   },
//	{ "DIVF",   OP_AFOP_F,  0, OPT_FPP,  0174400, 0000000, NULL   },
	{ "LDCff",  OP_AFOP,    0, OPT_FPP,  0177400, 0000377, NULL   },
//	{ "LDCDF",  OP_AFOP_D,  0, OPT_FPP,  0177400, 0000000, NULL   },
//	{ "LDCFD",  OP_AFOP_F,  0, OPT_FPP,  0177400, 0000000, NULL   },
	{ "LDCff",  OP_ASMD,    0, OPT_FPP,  0177000, 0000377, NULL   },
//	{ "LDCID",  OP_ASMD_D,  0, OPT_FPP,  0177000, 0000000, NULL   },
//	{ "LDCIF",  OP_ASMD_F,  0, OPT_FPP,  0177000, 0000000, NULL   },
//	{ "LDCLD",  OP_ASMD_DL, 0, OPT_FPP,  0177000, 0000000, NULL   },
//	{ "LDCLF",  OP_ASMD_FL, 0, OPT_FPP,  0177000, 0000000, NULL   },
	{ "LDf",    OP_AFOP,    0, OPT_FPP,  0172400, 0000377, NULL   },
//	{ "LDD",    OP_AFOP_D,  0, OPT_FPP,  0172400, 0000000, NULL   },
//	{ "LDF",    OP_AFOP_F,  0, OPT_FPP,  0172400, 0000000, NULL   },
	{ "LDEXP",  OP_ASOP,    0, OPT_FPP,  0176400, 0000000, NULL   },
	{ "LDFPS",  OP_SOP,     0, OPT_FPP,  0170100, 0000077, NULL   },
	{ "MODf",   OP_AFOP,    0, OPT_FPP,  0171400, 0000377, NULL   },
//	{ "MODD",   OP_AFOP_D,  0, OPT_FPP,  0171400, 0000000, NULL   },
//	{ "MODF",   OP_AFOP_F,  0, OPT_FPP,  0171400, 0000000, NULL   },
	{ "MULf",   OP_AFOP,    0, OPT_FPP,  0171000, 0000377, NULL   },
//	{ "MULD",   OP_AFOP_D,  0, OPT_FPP,  0171000, 0000000, NULL   },
//	{ "MULF",   OP_AFOP_F,  0, OPT_FPP,  0171000, 0000000, NULL   },
	{ "NEGf",   OP_FOP,     0, OPT_FPP,  0170700, 0000077, NULL   },
//	{ "NEGD",   OP_FOP_D,   0, OPT_FPP,  0170700, 0000000, NULL   },
//	{ "NEGF",   OP_FOP_F,   0, OPT_FPP,  0170700, 0000000, NULL   },
	{ "SETD",   OP_NPN,     0, OPT_FPP,  0170011, 0000000, NULL   },
	{ "SETF",   OP_NPN,     0, OPT_FPP,  0170001, 0000000, NULL   },
	{ "SETI",   OP_NPN,     0, OPT_FPP,  0170002, 0000000, NULL   },
	{ "SETL",   OP_NPN,     0, OPT_FPP,  0170012, 0000000, NULL   },
	{ "STCff",  OP_AFOP,    0, OPT_FPP,  0176000, 0000377, NULL   },
//	{ "STCDF",  OP_AFOP_D,  0, OPT_FPP,  0176000, 0000000, NULL   },
//	{ "STCFD",  OP_AFOP_F,  0, OPT_FPP,  0176000, 0000000, NULL   },
	{ "STCff",  OP_ASMD,    0, OPT_FPP,  0175400, 0000377, NULL   },
//	{ "STCDI",  OP_ASMD_D,  0, OPT_FPP,  0175400, 0000000, NULL   },
//	{ "STCDL",  OP_ASMD_DL, 0, OPT_FPP,  0175400, 0000000, NULL   },
//	{ "STCFI",  OP_ASMD_F,  0, OPT_FPP,  0175400, 0000000, NULL   },
//	{ "STCFL",  OP_ASMD_FL, 0, OPT_FPP,  0175400, 0000000, NULL   },
	{ "STf",    OP_AFOP,    0, OPT_FPP,  0174000, 0000377, NULL   },
//	{ "STD",    OP_AFOP_D,  0, OPT_FPP,  0174000, 0000000, NULL   },
//	{ "STF",    OP_AFOP_F,  0, OPT_FPP,  0174000, 0000000, NULL   },
	{ "STEXP",  OP_ASOP,    0, OPT_FPP,  0175000, 0000000, NULL   },
	{ "STFPS",  OP_SOP,     0, OPT_FPP,  0170200, 0000077, NULL   },
	{ "STST",   OP_SOP,     0, OPT_FPP,  0170300, 0000077, NULL   },
	{ "SUBf",   OP_AFOP,    0, OPT_FPP,  0173000, 0000377, NULL   },
//	{ "SUBD",   OP_AFOP_D,  0, OPT_FPP,  0173000, 0000000, NULL   },
//	{ "SUBF",   OP_AFOP_F,  0, OPT_FPP,  0173000, 0000000, NULL   },
	{ "TSTf",   OP_FOP,     0, OPT_FPP,  0170500, 0000077, NULL   },
//	{ "TSTD",   OP_FOP_D,   0, OPT_FPP,  0170500, 0000000, NULL   },
//	{ "TSTF",   OP_FOP_F,   0, OPT_FPP,  0170500, 0000000, NULL   },

	{ NULL } // Null Terminator
};
#endif


