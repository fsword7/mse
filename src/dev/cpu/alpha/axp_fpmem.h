/*
 * axp_fpmem.h - FP memory instructions
 *
 *  Created on: Feb 9, 2021
 *      Author: Tim Stark
 */

#pragma once

#define DOPC_LDF	FPSTART; if (FA != 31) FAV = readv32(RBV + SXTW(DISP16))
#define DOPC_LDG	FPSTART; if (FA != 31) FAV = readv64(RBV + SXTW(DISP16))
#define DOPC_LDS	FPSTART; if (FA != 31) FAV = readv32(RBV + SXTW(DISP16))
#define DOPC_LDT	FPSTART; if (FA != 31) FAV = readv64(RBV + SXTW(DISP16))

#define DOPC_STF	FPSTART; if (FA != 31) writev32(RBV + SXTW(DISP16), FAV)
#define DOPC_STG	FPSTART; if (FA != 31) writev64(RBV + SXTW(DISP16), FAV)
#define DOPC_STS	FPSTART; if (FA != 31) writev32(RBV + SXTW(DISP16), FAV)
#define DOPC_STT	FPSTART; if (FA != 31) writev64(RBV + SXTW(DISP16), FAV)
