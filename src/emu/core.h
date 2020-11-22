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

// extended signed definition
#define SXTB(val)	sint8_t(val)
#define SXTW(val)	sint16_t(val)
#define SXTL(val)	sint32_t(val)
#define SXTQ(val)	sint64_t(val)

// extended unsigned definition
#define ZXTB(val)	uint8_t(val)
#define ZXTW(val)	uint16_t(val)
#define ZXTL(val)	uint32_t(val)
#define ZXTQ(val)	uint64_t(val)
