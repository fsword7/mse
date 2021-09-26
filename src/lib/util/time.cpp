/*
 *  time.cpp - time in attoseconds package
 *
 *  Created on: Sep 23, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "lib/util/time.h"

using namespace emu::lib::util;

Attotime Attotime::zero  = Attotime(0);
Attotime Attotime::never = Attotime(ATTOTIME_MAX_SECONDS);
