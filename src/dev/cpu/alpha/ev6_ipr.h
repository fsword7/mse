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

#define IPR_DTB_TAG0						0x20
#define IPR_DTB_PTE0						0x21

#define IPR_DTB_IS0							0x24
#define IPR_DTB_ASN0						0x25
#define IPR_DTB_ALTMODE						0x26
#define IPR_MM_STAT							0x27
#define IPR_M_CTL							0x28
#define IPR_DC_CTL							0x29
#define IPR_DC_STAT							0x2A
#define IPR_C_DATA							0x2B
#define IPR_C_SHFT							0x2C

#define IPR_PCXT0							0x40
#define IPR_PCXT0_ASN						0x41
#define IPR_PCXT0_ASTER						0x42
#define IPR_PCXT0_ASTER_ASN					0x43
#define IPR_PCXT0_ASTRR						0x44
#define IPR_PCXT0_ASTRR_ASN					0x45
#define IPR_PCXT0_ASTRR_ASTER				0x46
#define IPR_PCXT0_ASTRR_ASTER_ASN			0x47
#define IPR_PCXT0_PPCE						0x48
#define IPR_PCXT0_PPCE_ASN					0x49
#define IPR_PCXT0_PPCE_ASTER				0x4A
#define IPR_PCXT0_PPCE_ASTER_ASN			0x4B
#define IPR_PCXT0_PPCE_ASTRR				0x4C
#define IPR_PCXT0_PPCE_ASTRR_ASN			0x4D
#define IPR_PCXT0_PPCE_ASTRR_ASTER			0x4E
#define IPR_PCXT0_PPCE_ASTRR_ASTER_ASN		0x4F
#define IPR_PCXT0_FPE						0x50
#define IPR_PCXT0_FPE_ASN					0x51
#define IPR_PCXT0_FPE_ASTER					0x52
#define IPR_PCXT0_FPE_ASTER_ASN				0x53
#define IPR_PCXT0_FPE_ASTRR					0x54
#define IPR_PCXT0_FPE_ASTRR_ASN				0x55
#define IPR_PCXT0_FPE_ASTRR_ASTER			0x56
#define IPR_PCXT0_FPE_ASTRR_ASTER_ASN		0x57
#define IPR_PCXT0_FPE_PPCE					0x58
#define IPR_PCXT0_FPE_PPCE_ASN				0x59
#define IPR_PCXT0_FPE_PPCE_ASTER			0x5A
#define IPR_PCXT0_FPE_PPCE_ASTER_ASN		0x5B
#define IPR_PCXT0_FPE_PPCE_ASTRR			0x5C
#define IPR_PCXT0_FPE_PPCE_ASTRR_ASN		0x5D
#define IPR_PCXT0_FPE_PPCE_ASTRR_ASTER		0x5E
#define IPR_PCXT0_FPE_PPCE_ASTRR_ASTER_ASN	0x5F

#define IPR_PCXT1							0x60
#define IPR_PCXT1_ASN						0x61
#define IPR_PCXT1_ASTER						0x62
#define IPR_PCXT1_ASTER_ASN					0x63
#define IPR_PCXT1_ASTRR						0x64
#define IPR_PCXT1_ASTRR_ASN					0x65
#define IPR_PCXT1_ASTRR_ASTER				0x66
#define IPR_PCXT1_ASTRR_ASTER_ASN			0x67
#define IPR_PCXT1_PPCE						0x68
#define IPR_PCXT1_PPCE_ASN					0x69
#define IPR_PCXT1_PPCE_ASTER				0x6A
#define IPR_PCXT1_PPCE_ASTER_ASN			0x6B
#define IPR_PCXT1_PPCE_ASTRR				0x6C
#define IPR_PCXT1_PPCE_ASTRR_ASN			0x6D
#define IPR_PCXT1_PPCE_ASTRR_ASTER			0x6E
#define IPR_PCXT1_PPCE_ASTRR_ASTER_ASN		0x6F
#define IPR_PCXT1_FPE						0x70
#define IPR_PCXT1_FPE_ASN					0x71
#define IPR_PCXT1_FPE_ASTER					0x72
#define IPR_PCXT1_FPE_ASTER_ASN				0x73
#define IPR_PCXT1_FPE_ASTRR					0x74
#define IPR_PCXT1_FPE_ASTRR_ASN				0x75
#define IPR_PCXT1_FPE_ASTRR_ASTER			0x76
#define IPR_PCXT1_FPE_ASTRR_ASTER_ASN		0x77
#define IPR_PCXT1_FPE_PPCE					0x78
#define IPR_PCXT1_FPE_PPCE_ASN				0x79
#define IPR_PCXT1_FPE_PPCE_ASTER			0x7A
#define IPR_PCXT1_FPE_PPCE_ASTER_ASN		0x7B
#define IPR_PCXT1_FPE_PPCE_ASTRR			0x7C
#define IPR_PCXT1_FPE_PPCE_ASTRR_ASN		0x7D
#define IPR_PCXT1_FPE_PPCE_ASTRR_ASTER		0x7E
#define IPR_PCXT1_FPE_PPCE_ASTRR_ASTER_ASN	0x7F
