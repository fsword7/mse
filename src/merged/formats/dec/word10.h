/*
 * word10.h
 *
 *  Created on: Sep 6, 2015
 *      Author: Tim Stark
 */

#pragma once

typedef uint32_t   uint18_t;
typedef uint64_t   uint36_t;

typedef int32_t   sint18_t;
typedef int64_t   sint36_t;

typedef uint18_t   h10_t;
typedef uint36_t   w10_t;

#define H10_WIDTH   18
#define H10_MASK    0777777

#define W10_WIDTH    (H10_WIDTH << 2)
#define W10_FMASK    0777777777777LL
#define W10_CMASK    0377777777777LL
#define W10_SIGN     0400000000000LL
#define W10_C1MASK  01000000000000LL
#define W10_LMASK    0777777000000LL
#define W10_RMASK    0000000777777LL

#define W10_LEFT(d)  (((d) >> H10_WIDTH) & H10_MASK)
#define W10_RIGHT(d) ((d) & H10_MASK)

#define W10_SWAP(var) (((var >> H10_WIDTH) & W10_RMASK) | ((var & W10_RMASK) << H10_WIDTH))
