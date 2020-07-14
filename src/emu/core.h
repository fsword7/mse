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
