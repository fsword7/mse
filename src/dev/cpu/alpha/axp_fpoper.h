/*
 * axp_fpoper.h - Operate instructions (floating)
 *
 *  Created on: Feb 9, 2021
 *      Author: Tim Stark
 */

#pragma once

#define DOPC_MF_FPCR	FPSTART; FAV = state.fpcr;

#define DOPC_MT_FPCR                            \
	FPSTART;                                    \
	state.fpcr = FAV & 0x7FFF800000000000ull;   \
	if (state.fpcr & 0x3F00000000000000ull)     \
		state.fpcr |= 0x8000000000000000ull;
