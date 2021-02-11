/*
 * axp_mmu.h - Alpha series - memory management
 *
 *  Created on: Feb 11, 2021
 *      Author: Tim Stark
 */

#pragma once

#define SPE0_MASK  0x0000FFFF'C0000000LL // Virtual Address <47:30>
#define SPE0_MATCH 0x0000FFF8'00000000LL // Address match   <47:31>
#define SPE0_MAP   0x00000000'3FFFFFFFLL // Map address     <29:0>

#define SPE1_MASK  0x0000FE00'00000000LL // Virtual address <47:41>
#define SPE1_MATCH 0x0000FC00'00000000LL // Address match   <47:42>
#define SPE1_MAP   0x000001FF'FFFFFFFFLL // Map address     <40:0>
#define SPE1_TEST  0x00000100'00000000LL // Test address    <40>
#define SPE1_ADD   0x00000E00'00000000LL // Add address     <43:41>

#define SPE2_MASK  0x0000C000'00000000LL // Virtual address <47:46>
#define SPE2_MATCH 0x00008000'00000000LL // Address match   <47>
#define SPE2_MAP   0x00000FFF'00000000LL // Map address     <43:0>
