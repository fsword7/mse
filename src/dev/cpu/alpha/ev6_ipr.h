/*
 * ev6_ipr.h - 21264 series - EV6 internal processor registers
 *
 *  Created on: Feb 6, 2021
 *      Author: Tim Stark
 */

#pragma once

#define ICTL_BP_MODE_FALL		0x2		// 1x, where 'x' is not relevant
#define ICTL_BP_MODE_DYN		0x0		// 0x, where 'x' is relevant
#define ICTL_BP_MODE_LOCAL		0x1		// Local history prediction
#define ICTL_BP_MODE_CHOICE 	0x0		// Choice selected prediction

#define ICTL_SDE_ENABLE			0x2		// bit 0 does not affect 21264 operation

#define PAL_BASE_MASK  0x00000FFFFFFF8000ull


// IPR index definitions

#define IPR_ITB_TAG							0x00
#define IPR_ITB_PTE							0x01
#define IPR_ITB_IAP							0x02
#define IPR_ITB_IA							0x03
#define IPR_ITB_IS							0x04

#define IPR_EXC_ADDR						0x06
#define IPR_IVA_FORM						0x07

#define IPR_CM								0x09
#define IPR_IER								0x0A
#define IPR_IER_CM							0x0B
#define IPR_SIRR							0x0C
#define IPR_ISUM							0x0D
#define IPR_HW_INT_CLR						0x0E
#define IPR_EXC_SUM							0x0F
#define IPR_PAL_BASE						0x10
#define IPR_I_CTL							0x11
#define IPR_IC_FLUSH_ASM					0x12
#define IPR_IC_FLUSH						0x13
#define IPR_PCTR_CTL						0x14
#define IPR_CLR_MAP							0x15
#define IPR_I_STAT							0x16
#define IPR_SLEEP							0x17

#define IPR_DTB_IA							0xA3
#define IPR_DTB_IAP							0xA2
#define IPR_DTB_IS0							0x24
#define IPR_DTB_IS1							0xA4
#define IPR_DTB_TAG0						0x20
#define IPR_DTB_TAG1						0xA0
#define IPR_DTB_PTE0						0x21
#define IPR_DTB_PTE1						0xA1
#define IPR_DTB_ASN0						0x25
#define IPR_DTB_ASN1						0xA5
#define IPR_DTB_ALTMODE						0x26

#define IPR_MM_STAT							0x27
#define IPR_M_CTL							0x28
#define IPR_DC_CTL							0x29
#define IPR_DC_STAT							0x2A
#define IPR_C_DATA							0x2B
#define IPR_C_SHFT							0x2C

#define IPR_PCXT0							0x40
#define IPR_PCXT1							0x60

#define PCXT_ASN
#define PCXT_ASTER
#define PCXT_ASTRR
#define PCXT_PPCE
#define PCXT_FPE

// Ebox IPR registers
#define IPR_CC								0xC0
#define IPR_CC_CTL							0xC1
#define IPR_VA								0xC2
#define IPR_VA_FORM							0xC3
#define IPR_VA_CTL							0xC4
