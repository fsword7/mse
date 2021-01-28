/*
 * axp_arith.h - Alpha CPU processor - arithmetic instructions
 *
 *  Created on: Nov 21, 2020
 *      Author: Tim Stark
 */

#pragma once

// Addition instructions
#define DOPC_ADDL		RCV = SXTL(RAV + RBVL)
#define DOPC_S4ADDL		RCV = SXTL((RAV * 4) + RBVL)
#define DOPC_S8ADDL		RCV = SXTL((RAV * 8) + RBVL)

#define DOPC_ADDQ		RCV = RAV + RBVL
#define DOPC_S4ADDQ		RCV = (RAV * 4) + RBVL
#define DOPC_S8ADDQ		RCV = (RAV * 8) + RBVL

// Subtract instructions
#define DOPC_SUBL		RCV = SXTL(RAV - RBVL)
#define DOPC_S4SUBL		RCV = SXTL((RAV * 4) - RBVL)
#define DOPC_S8SUBL		RCV = SXTL((RAV * 8) - RBVL)

#define DOPC_SUBQ		RCV = RAV - RBVL
#define DOPC_S4SUBQ		RCV = (RAV * 4) - RBVL
#define DOPC_S8SUBQ		RCV = (RAV * 8) - RBVL

// Multiply instructions
#define DOPC_MULL		RCV = SXTL(SXTL(RAV) * SXTL(RBVL))
#define DOPC_MULQ		RCV = RAV * RBVL

// Comparison instructions
#define DOPC_CMPEQ		RCV = (RAV == RBVL) ? 1 : 0
#define DOPC_CMPULE		RCV = (ZXTQ(RAV) < ZXTQ(RBVL)) ? 1 : 0
#define DOPC_CMPULT		RCV = (ZXTQ(RAV) <= ZXTQ(RBVL)) ? 1 : 0
#define DOPC_CMPLT		RCV = (SXTQ(RAV) < SXTQ(RBVL)) ? 1 : 0
#define DOPC_CMPLE		RCV = (SXTQ(RAV) <= SXTQ(RBVL)) ? 1 : 0
