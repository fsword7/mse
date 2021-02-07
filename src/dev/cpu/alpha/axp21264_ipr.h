/*
 * axp21264_ipr.h - Alpha 21264 - IPR registers
 *
 *  Created on: Feb 6, 2021
 *      Author: Tim Stark
 */

#pragma once

// I_CTL - Ibox Control register
//
// Chip ID field following values:
//
//	3 = 21264 pass 2.3
//	5 = 21264 pass 2.5

struct iCtl_t
{
	uint64_t spce:1;			// System performance counter enable
	uint64_t ic_en:2;			// ICache set enable
	uint64_t spe:3;				// Super page mode enable
	uint64_t sde:2;				// PALshadow register enable
	uint64_t sbe:2;				// Stream buffer enable
	uint64_t bpMode:2;			// Brach prediction mode selection
	uint64_t hwe:1;				// Allow PAL reserved opcodes in kernel
	uint64_t sl_xmit:1;			// Cause SROM to advance to next bit
	uint64_t sl_rcv:1;
	uint64_t va48:1;			// Enable 48-bit addresses (45 otherwise)
	uint64_t vaForm32:1;		// Address formatting on read of IVA_FORM
	uint64_t single_issue_h:1;	// Force instruction issue from bottom of queues
	uint64_t pct0_en:1;			// Enable performance counter #0
	uint64_t pct1_en:1;			// Enable performance counter #1
	uint64_t call_pal_r23:1;	// Use PALshadow register R23, instead of R27
	uint64_t mchk_en:1;			// Machine check enable
	uint64_t tb_mb_en:1;		// Insert MB on TB fills (1 = multiprocessors)
	uint64_t bist_fail:1;		// Indicates status of BIST (1 = pass / 0 = fail)
	uint64_t chip_id:6;			// Chip revision ID
	uint64_t vptb:18;			// Virtual page table base
	uint64_t sext_vptb:16;		// Sign extension of VPTB
};

#define READ_ICTL(dest) \
	{ union { uint64_t tmp1; iCtl_t tmp2; } src = { .tmp2 = state.ictl }; dest = src.tmp1; }

#define WRITE_ICTL(src) \
	{ union { uint64_t tmp1; iCtl_t tmp2; } dest = { .tmp1 = src }; state.ictl = dest.tmp2; }
