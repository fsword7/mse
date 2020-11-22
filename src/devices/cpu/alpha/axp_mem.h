/*
 * axp_memory.h - Alpha CPU Processor - memory instructions
 *
 *  Created on: Nov 21, 2020
 *      Author: Tim Stark
 */

#pragma once

// load address instructions
#define DO_LDA		RAV = RBV + DISP16
#define DO_LDAH		RAV = RBV + (DISP16 << 16)

// load instructions
#define DO_LDBU		READV(RBV + DISP16, 8, RAV)
#define DO_LDWU		READV(RBV + DISP16, 16, RAV)
#define DO_LDL		READV(RBV + DISP16, 32, RAV)
#define DO_LDL_L	READVL(RBV + DISP16, 32 RAV)
#define DO_LDQ		READV(RBV + DISP16, 64, RAV)
#define DO_LDQ_L	READVL(RBV + DISP16, 64, RAV)
#define DO_LDQ_U	READV((RBV + DISP16) & ~0x07LL, 64, RAV)

// store instructions
#define DO_STB		WRITEV(RBV + DISP16, 8, RAV)
#define DO_STW		WRITEV(RBV + DISP16, 16, RAV)
#define DO_STL		WRITEV(RBV + DISP16, 32, RAV)
#define DO_STL_C	WRITEVC(RBV + DISP16, 32, RAV)
#define DO_STQ		WRITEV(RBV + DISP16, 64, RAV)
#define DO_STQ_C	WRITEVC(RBV + DISP16, 64, RAV)
#define DO_STQ_U	WRITEV((RBV + DISP16) & ~0x07LL, 64, RAV)
