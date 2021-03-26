/*
 * core.h
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <typeinfo>
#include <thread>

#include <vector>
#include <map>

#include <cassert>

#include <fmt/printf.h>

// LSB_FIRST endianess (1 = little endianess, 0 = big endianess)
#define LSB_FIRST 1

enum endian_t { LittleEndian, BigEndian };

#if LSB_FIRST
constexpr const endian_t endianNative = LittleEndian;
#else
constexpr const endian_t endianNative = BigEndian;
#endif

// Determine endian-based values
#define ENDIAN_VALUE(endian, leValue, beValue)	((endian == LittleEndian) ? (leValue) : (beValue))
#define NATIVE_ENDIAN(leValue, beValue)			ENDIAN_VALUE(endianNative, leValue, beValue)


using namespace std;

namespace fs = std::filesystem;

using offs_t = uint64_t;

typedef const char tag_t;
typedef const char ctag_t;
typedef const char cchar_t;
typedef string stag_t;
typedef const string cstag_t;

// Common include packages
#include "emu/debug.h"
#include "emu/console.h"
#include "emu/sysconfig.h"
#include "emu/device.h"
#include "emu/map/map.h"

//#include "emu/machine.h"

#include "emu/templates.h"

// Data access length
#define LN_BYTE		1
#define LN_WORD		2
#define LN_LONG		4
#define LN_QUAD		8
#define LN_OCTA		16

// sign extension definition
#define SXTB(val)	int8_t(val)
#define SXTW(val)	int16_t(val)
#define SXTL(val)	int32_t(val)
#define SXTQ(val)	int64_t(val)

// zero extension definition
#define ZXTB(val)	uint8_t(val)
#define ZXTW(val)	uint16_t(val)
#define ZXTL(val)	uint32_t(val)
#define ZXTQ(val)	uint64_t(val)

// Varying pair/scale 16-bit integer
union pair16_t {
#if LSB_FIRST
	struct { int8_t l, h; }  sb;
	struct { uint8_t l, h; } ub;
#else
	struct { int8_t h, l; }  sb;
	struct { uint8_t h, l; } ub;
#endif
	int16_t  sw;
	uint16_t uw;
};

// Varying pair/scale 32-bit integer
union pair32_t {
#if LSB_FIRST
	struct { int8_t l, h, h2, h3; }  sb;
	struct { uint8_t l, h, h2, h3; } ub;
	struct { int16_t l, h; }  sw;
	struct { uint16_t l, h; } uw;
#else
	struct { int8_t h3, h2, h, l; }  sb;
	struct { uint8_t h3, h2, h, l; } ub;
	struct { int16_t h, l; }  sw;
	struct { uint16_t h, l; } uw;
#endif
	int32_t  sd;
	uint32_t ud;
};

// Varying pair/scale 64-bit integer
union pair64_t {
#if LSB_FIRST
	struct { int8_t l, h, h2, h3, h4, h5, h6, h7; }  sb;
	struct { uint8_t l, h, h2, h3, h4, h5, h6, h7; } ub;
	struct { int16_t l, h, h2, h3; }  sw;
	struct { uint16_t l, h, h2, h3; } uw;
	struct { int32_t l, h; }  sd;
	struct { uint32_t l, h; } ud;
#else
	struct { int8_t h7, h6, h5. h4, h3, h2, h, l; }  sb;
	struct { uint8_t h7, h6. h5, h4, h3, h2, h, l; } ub;
	struct { int16_t h3, h2, h, l; }  sw;
	struct { uint16_t h3, h2, h, l; } uw;
	struct { int32_t h, l; }  sd;
	struct { uint32_t h, l; } ud;
#endif
	int64_t  sq;
	uint64_t uq;
};

// Varying scale unsigned integer
struct scale16_t {
	union {
		uint16_t w; // Word access
		uint8_t  b; // Byte access
	};
};

// Varying scale unsigned integer
struct scale32_t {
	union {
		uint32_t l; // Longword access
		uint16_t w; // Word access
		uint8_t  b; // Byte access
	};
};

// Varying scale unsigned integer
struct scale64_t {
	union {
		uint64_t q; // Quadword access
		uint32_t l; // Longword access
		uint16_t w; // Word access
		uint8_t  b; // Byte access
	};
};
