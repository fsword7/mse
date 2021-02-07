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

using namespace std;

namespace fs = std::filesystem;

using offs_t = uint64_t;

typedef const char tag_t;
typedef const char ctag_t;
typedef string stag_t;
typedef const string cstag_t;

// Endianess type (little or big)
enum endian_t { LittleEndian, BigEndian };

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
