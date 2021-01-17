/*
 * opcodes.h
 *
 *  Created on: Jan 17, 2021
 *      Author: Tim Stark
 */

// Opcode field types
#define OPT_PAL		0		// PALcode instructions
#define OPT_BRA		1		// Branch instructions
#define OPT_MEM		2		// Memory instructions
#define OPT_IOP		3		// Operate instructions (integer)
#define OPT_FOP		4		// Floating instructions

// Operand modes
#define OPR_PAL		0x0200	// PALcode number
#define OPR_MJP		0x0100  // Jump displacement
#define OPR_MDP		0x0080  // Memory displacement
#define OPR_BDP		0x0040  // Branch displacement
#define OPR_IDX		0x0020  // Register B - index
#define OPR_LIT		0x0010  // 8-bit literal
#define OPR_RC		0x0004	// Register C
#define OPR_RB		0x0002  // Register B
#define OPR_RA		0X0001  // Register A
#define OPR_NONE	0x0000  // Hardware PALcode

#define OP_BRA	OPT_BRA|OPR_RA|OPR_BDP

#define OPC_BRA(opc)	OP_BRA, opc, 0

