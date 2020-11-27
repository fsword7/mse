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

#include <vector>

#include <cassert>

#include <fmt/printf.h>

using namespace std;

namespace fs = std::filesystem;

typedef const char tag_t;

// Common include packages
#include "emu/sysconfig.h"
#include "emu/device.h"

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
