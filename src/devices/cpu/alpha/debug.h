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

#define POST_X8(VAL)  fmt::sprintf(" ==> %02X", ZEXTB(val))
#define POST_X16(val) fmt::sprintf(" ==> %04X", ZEXTW(val))
#define POST_X32(val) fmt::sprintf(" ==> %08X", ZEXTL(val))
#define POST_X64(val) fmt::sprintf(" ==> %016llX", ZEXTQ(val))

#define PRE_MEM(opcode) 																					\
	{																										\
		dbgout += fmt::sprintf("%016llX %-8s r%d, %04xH(r%d)", state.vpcReg, #opcode, RA, ZETW(inst), RB);	\
		dbgout += fmt::sprintf(" : (%016llX)", RBV);														\
	}


#else

#define UNKNOWN_OPCODE1
#define UNKNOWN_OPCODE2

#define OPC_EXEC(opcode, format) DOPC_##opcode
#define OPC_FUNC(opcode, format)

#endif
