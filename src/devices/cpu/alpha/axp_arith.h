/*
 * axp_arith.h - Alpha CPU processor - arithmetic instructions
 *
 *  Created on: Nov 21, 2020
 *      Author: Tim Stark
 */

#pragma once

// Addition instructions
#define DO_ADDL		RCV = SXTL(RAV + RBV)
#define DO_S4ADDL	RCV = SXTL((RAV * 4) + RBV)
#define DO_S8ADDL	RCV = SXTL((RAV * 8) + RBV)

#define DO_ADDQ		RCV = RAV + RBV
#define DO_S4ADDQ	RCV = (RAV * 4) + RBV
#define DO_S8ADDQ	RCV = (RAV * 8) + RBV

// Subtract instructions
#define DO_SUBL		RCV = SXTL(RAV - RBV)
#define DO_S4SUBL	RCV = SXTL((RAV * 4) - RBV)
#define DO_S8SUBL	RCV = SXTL((RAV * 8) - RBV)

#define DO_SUBQ		RCV = RAV - RBV
#define DO_S4SUBQ	RCV = (RAV * 4) - RBV
#define DO_S8SUBQ	RCV = (RAV * 8) - RBV

// Multiply instructions
#define DO_MULL		RCV = SXTL(SXTL(RAV) * SXTL(RBV))
#define DO_MULQ		RCV = RAV * RBV

// Comparison instructions
#define DO_CMPEQ	RVC = (RAV == RBV) ? 1 : 0
#define DO_CMPULE	RVC = (ZXTQ(RAV) < ZXTQ(RBV)) ? 1 : 0
#define DO_CMPULT	RVC = (ZXTQ(RAV) <= ZXTQ(RBV)) ? 1 : 0
#define DO_CMPLT	RVC = (SXTQ(RAV) < SXTQ(RBV)) ? 1 : 0
#define DO_CMPLE	RVC = (SXTQ(RAV) <= SXTQ(RBV)) ? 1 : 0
