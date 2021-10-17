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

cstag_t Attotime::getAsString(int precision) const
{
    string str;

    if (*this == never)
        str = fmt::sprintf("%-*s", precision, "(never)");
    else if (precision == 0) // seconds only
        str = fmt::sprintf("%lld", attoseconds / ATTOSECONDS_PER_SECOND);
    else if (precision <= 9)
    {
        uint64_t upper = attoseconds / ATTOSECONDS_PER_NANOSECOND;
        uint64_t temp = precision;
        while (temp < 9)
        {
            upper /= 10;
            temp++;
        }
        str = fmt::sprintf("%lld.%0*lld", attoseconds / ATTOSECONDS_PER_SECOND, precision, upper);
    }

    return str;
}