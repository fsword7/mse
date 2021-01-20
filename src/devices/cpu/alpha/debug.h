/*
 * debug.h - Alpha CPU Processor - debugging routines
 *
 *  Created on: Nov 21, 2020
 *      Author: Tim Stark
 */

#pragma once

#ifdef DEBUG

#define UNKNOWN_OPCODE1 \
	fmt::printf("*** Unknown opcode %02X\n", opCode);

#define UNKNOWN_OPCODE2 \
	fmt::printf("*** Unknown opcode %02X.%02X\n", opCode, func);

#define POST_X8(VAL)  dbgout += fmt::sprintf(" ==> %02X", ZXTB(val))
#define POST_X16(val) dbgout += fmt::sprintf(" ==> %04X", ZXTW(val))
#define POST_X32(val) dbgout += fmt::sprintf(" ==> %08X", ZXTL(val))
#define POST_X64(val) dbgout += fmt::sprintf(" ==> %016llX", ZXTQ(val))


// Condition instructions
#define PRE_COND(opcode)                                                \
		dbgval = (state.vpcReg + (DISP21 << 2)) & ~0x3;                 \
		dbgout += fmt::sprintf("%016llx %-8s r%d, %016llX : (%016llX)", \
			state.vpcReg-4, #opcode, RA, dbgval, RAV);

#define POST_COND

// Mmemory instructions
#define PRE_MEM(opcode) 																					 \
	{																										 \
		dbgout += fmt::sprintf("%016llX %-8s r%d, %04xH(r%d)", state.vpcReg-4, #opcode, RA, ZXTW(opWord), RB); \
		dbgout += fmt::sprintf(" : (%016llX)", RBV);														 \
	}

#define POST_MEM POST_X64(RAV)


#define OPC_EXEC(opcode, format) DOPC_##opcode
#define OPC_FUNC(opcode, format)

#define OPC_EXEC2(opcode, format) \
	PRE_##format(opcode);         \
	DOPC_##opcode;                \
	POST_##format;                \
	cout << dbgout << endl;

#else

#define UNKNOWN_OPCODE1
#define UNKNOWN_OPCODE2

#define OPC_EXEC(opcode, format) DOPC_##opcode
#define OPC_FUNC(opcode, format)

#endif
