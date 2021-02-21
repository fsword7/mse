/*
 * int36.h - Octal-based 18/36-bit integer package
 *
 *  Created on: Nov 3, 2019
 *      Author: Tim Stark
 */

#pragma once

#define I18_BITS			18
#define I18_FMASK			00777777
#define I18_CMASK			00377777
#define I18_CRYMASK			01000000

#define I36_BITS			36
#define I36_FMASK			00777777777777LL
#define I36_CMASK			00377777777777LL
#define I36_CRYMASK			01000000000000LL
#define I36_LMASK			00777777000000LL
#define I36_LCMASK			00377777000000LL
#define I36_RMASK			00000000777777LL
#define I36_RCMASK			00000000377777LL

#define op36_invert(x)		((x) ^ I36_CMASK)
#define op36_negate(x)		(op36_invert(x) + 1)
#define op36_swap(x)		(((x) & I36_LMASK) >> I18_BITS) | ((x) & I36_RMASK) << I18_BITS))

typedef uint32_t	uint18_t;
typedef uint64_t	uint36_t;
typedef int32_t		sint18_t;
typedef int64_t		sint36_t;
