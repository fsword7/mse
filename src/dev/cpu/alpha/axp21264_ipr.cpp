/*
 * axp21264_ipr.cpp
 *
 *  Created on: Feb 6, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"

static ctag_t *iprNames[] =
{
		"ITB_TAG",		"ITB_PTE",		"ITB_IAP",		"ITB_IA",
		"ITB_IS",		"",				"EXC_ADDR",		"IVA_FORM",
		"",				"CM",			"IER",			"IER_CM",
		"SIRR",			"ISUM",			"HW_INT_CLR",	"EXC_SUM",
		"PAL_BASE",		"I_CTL",		"IC_FLUSH_ASM",	"IC_FLUSH",
		"PCTR_CTL",		"CLR_MAP",		"I_STAT",		"SLEEP",
		"",				"",				"",				"",
		"",				"",				"",				"",
		"DTB_TAG0",		"DTB_PTE0",		"",				"",
		"DTB_IS0",		"DTB_ASN0",		"DTB_ALTMODE",	"MM_STAT",
		"M_CTL",		"DC_CTL",		"DC_STAT",		"C_DATA",
		"C_SHFT",		"",				"",				"",
		"",				"",				"",				"",
		"",				"",				"",				"",
		"",				"",				"",				"",
		"",				"",				"",				""

		"PCXT0",						"PCXT0_ASN",
		"PCXT0_ASTER",					"PCXT0_ASTER_ASN",
		"PCXT0_ASTRR",					"PCXT0_ASTRR_ASN",
		"PCXT0_ASTRR_ASTER",			"PCXT0_ASTRR_ASTER_ASN",
		"PCXT0_PPCE",					"PCXT0_PPCE_ASN",
		"PCXT0_PPCE_ASTER",				"PCXT0_PPCE_ASTER_ASN",
		"PCXT0_PPCE_ASTRR",				"PCXT0_PPCE_ASTRR_ASN",
		"PCXT0_PPCE_ASTRR_ASTER",		"PCXT0_PPCE_ASTRR_ASTER_ASN",
		"PCXT0_FPE",					"PCXT0_FPE_ASN",
		"PCXT0_FPE_ASTER",				"PCXT0_FPE_ASTER_ASN",
		"PCXT0_FPE_ASTRR",				"PCXT0_FPE_ASTRR_ASN",
		"PCXT0_FPE_ASTRR_ASTER",		"PCXT0_FPE_ASTRR_ASTER_ASN",
		"PCXT0_FPE_PPCE",				"PCXT0_FPE_PPCE_ASN",
		"PCXT0_FPE_PPCE_ASTER",			"PCXT0_FPE_PPCE_ASTER_ASN",
		"PCXT0_FPE_PPCE_ASTRR",			"PCXT0_FPE_PPCE_ASTRR_ASN",
		"PCXT0_FPE_PPCE_ASTRR_ASTER",	"PCXT0_FPE_PPCE_ASTRR_ASTER_ASN",

		"PCXT1",						"PCXT1_ASN",
		"PCXT1_ASTER",					"PCXT1_ASTER_ASN",
		"PCXT1_ASTRR",					"PCXT1_ASTRR_ASN",
		"PCXT1_ASTRR_ASTER",			"PCXT1_ASTRR_ASTER_ASN",
		"PCXT1_PPCE",					"PCXT1_PPCE_ASN",
		"PCXT1_PPCE_ASTER",				"PCXT1_PPCE_ASTER_ASN",
		"PCXT1_PPCE_ASTRR",				"PCXT1_PPCE_ASTRR_ASN",
		"PCXT1_PPCE_ASTRR_ASTER",		"PCXT1_PPCE_ASTRR_ASTER_ASN",
		"PCXT1_FPE",					"PCXT1_FPE_ASN",
		"PCXT1_FPE_ASTER",				"PCXT1_FPE_ASTER_ASN",
		"PCXT1_FPE_ASTRR",				"PCXT1_FPE_ASTRR_ASN",
		"PCXT1_FPE_ASTRR_ASTER",		"PCXT1_FPE_ASTRR_ASTER_ASN",
		"PCXT1_FPE_PPCE",				"PCXT1_FPE_PPCE_ASN",
		"PCXT1_FPE_PPCE_ASTER",			"PCXT1_FPE_PPCE_ASTER_ASN",
		"PCXT1_FPE_PPCE_ASTRR",			"PCXT1_FPE_PPCE_ASTRR_ASN",
		"PCXT1_FPE_PPCE_ASTRR_ASTER",	"PCXT1_FPE_PPCE_ASTRR_ASTER_ASN"

};
