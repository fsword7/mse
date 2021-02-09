/*
 * ev5_defs.h - EV5 definitions
 *
 *  Created on: Feb 9, 2021
 *      Author: Tim Stark
 */

#pragma once

// HW_LD/HW_ST Flags
#define HW_PHYS  0x8000 // Physical/Virtual Access
#define HW_ALT   0x4000 //
#define HW_WRTCK 0x2000
#define HW_QUAD  0x1000 // Quadword/Longword Length
#define HW_VPTE  0x0800
#define HW_LOCK  0x0400
#define HW_DISP  0x03FF // 10-bit displacement


// IBOX Control and Status Register (ICSR)
// For more information, Check pages 5-20 to 5-22
// (Section 5.1.17) on 21164 Hardware Reference Manual.
#define ICSR_TST   0x0000008000000000LL // (RW,0) Send Test Signal
#define ICSR_ISTA  0x0000004000000000LL // (RO)   ICACHE BIST Status
#define ICSR_MBO   0x0000002000000000LL // (RW,1) Must Be One
#define ICSR_FBD   0x0000001000000000LL // (RW,0) Force Bad Data Parity
#define ICSR_FBT   0x0000000800000000LL // (RW,0) Force Bad Tag Parity
#define ICSR_FMS   0x0000000400000000LL // (RW,0) Force Miss
#define ICSR_SLE   0x0000000200000000LL // (RW,0) Serial Line Enable
#define ICSR_CRDE  0x0000000100000000LL // (RW,0) Error Interrupt Enable
#define ICSR_SDE   0x0000000040000000LL // (RW,0) Shadow PAL Register Enable
#define ICSR_SPE   0x0000000030000000LL // (RW,0) Super Page Mapping Enable
#define ICSR_HWE   0x0000000008000000LL // (RW,0) Hardware PAL Enable
#define ICSR_FPE   0x0000000004000000LL // (RW,0) Floating-Point Enable
#define ICSR_TMD   0x0000000002000000LL // (RW,0) Timeout Counter Disable
#define ICSR_TMM   0x0000000001000000LL // (RW,0) Timeout Counter Mode
#define ICSR_IMSK  0x0000000000F00000LL // (RW,0) Interrupt Mask Disable
#define ICSR_BSE   0x0000000000020000LL // (RW,0) Byte/Word Enable
#define ICSR_PME   0x0000000000000300LL // (RW,0) Performance Meter Enable

#define ICSR_RW    0x0000009FFFF20300LL // Writable Mask
