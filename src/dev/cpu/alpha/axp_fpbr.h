/*
 * axp_fpbranch.h - branch instructions (floating)
 *
 *  Created on: Feb 9, 2021
 *      Author: Tim Stark
 */

#pragma once

#define FPR_SIGN 0x8000000000000000ull

#define DOPC_FBEQ	FPSTART; if ((FAV & ~FPR_SIGN) == 0)         addPC(DISP21 << 2)
#define DOPC_FBNE	FPSTART; if ((FAV & ~FPR_SIGN) != 0)         addPC(DISP21 << 2)
#define DOPC_FBLT	FPSTART; if (FAV > FPR_SIGN)                 addPC(DISP21 << 2)
#define DOPC_FBLE	FPSTART; if ((FAV & FPR_SIGN) || (FAV == 0)) addPC(DISP21 << 2)
#define DOPC_FBGT	FPSTART; if ((FAV & FPR_SIGN) && (FAV != 0)) addPC(DISP21 << 2)
#define DOPC_FBGE	FPSTART; if (FAV <= FPR_SIGN)                addPC(DISP21 << 2)
