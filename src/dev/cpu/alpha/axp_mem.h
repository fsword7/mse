/*
 * axp_memory.h - Alpha CPU Processor - memory instructions
 *
 *  Created on: Nov 21, 2020
 *      Author: Tim Stark
 */

#pragma once

// load address instructions
#define DOPC_LDA		RAV = RBV + SXTW(DISP16)
#define DOPC_LDAH		RAV = RBV + SXTL(DISP16 << 16)

// load instructions
#define DOPC_LDBU		RAV = readv8(RBV + SXTW(DISP16))
#define DOPC_LDWU		RAV = readv16(RBV + SXTW(DISP16))
#define DOPC_LDL		RAV = readv32(RBV + SXTW(DISP16))
#define DOPC_LDL_L		RAV = readv32(RBV + SXTW(DISP16))
#define DOPC_LDQ		RAV = readv64(RBV + SXTW(DISP16))
#define DOPC_LDQ_L		RAV = readv64(RBV + SXTW(DISP16))
#define DOPC_LDQ_U		RAV = readv64((RBV + SXTW(DISP16)) & ~0x07LL)

// store instructions
#define DOPC_STB		writev8(RBV + SXTW(DISP16), RAV)
#define DOPC_STW		writev16(RBV + SXTW(DISP16), RAV)
#define DOPC_STL		writev32(RBV + SXTW(DISP16), RAV)
#define DOPC_STL_C		writev32(RBV + SXTW(DISP16), RAV)
#define DOPC_STQ		writev64(RBV + SXTW(DISP16), RAV)
#define DOPC_STQ_C		writev64(RBV + SXTW(DISP16), RAV)
#define DOPC_STQ_U		writev64((RBV + SXTW(DISP16)) & ~0x07LL, RAV)
