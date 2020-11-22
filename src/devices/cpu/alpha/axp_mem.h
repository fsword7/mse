/*
 * axp_memory.h - Alpha CPU Processor - memory instructions
 *
 *  Created on: Nov 21, 2020
 *      Author: Tim Stark
 */

#pragma once

// load address instructions
#define DOPC_LDA		RAV = RBV + DISP16
#define DOPC_LDAH		RAV = RBV + (DISP16 << 16)

// load instructions
#define DOPC_LDBU		READV(RBV + DISP16, 8, RAV)
#define DOPC_LDWU		READV(RBV + DISP16, 16, RAV)
#define DOPC_LDL		READV(RBV + DISP16, 32, RAV)
#define DOPC_LDL_L		READVL(RBV + DISP16, 32, RAV)
#define DOPC_LDQ		READV(RBV + DISP16, 64, RAV)
#define DOPC_LDQ_L		READVL(RBV + DISP16, 64, RAV)
#define DOPC_LDQ_U		READV((RBV + DISP16) & ~0x07LL, 64, RAV)

// store instructions
#define DOPC_STB		WRITEV(RBV + DISP16, 8, RAV)
#define DOPC_STW		WRITEV(RBV + DISP16, 16, RAV)
#define DOPC_STL		WRITEV(RBV + DISP16, 32, RAV)
#define DOPC_STL_C		WRITEVC(RBV + DISP16, 32, RAV)
#define DOPC_STQ		WRITEV(RBV + DISP16, 64, RAV)
#define DOPC_STQ_C		WRITEVC(RBV + DISP16, 64, RAV)
#define DOPC_STQ_U		WRITEV((RBV + DISP16) & ~0x07LL, 64, RAV)
