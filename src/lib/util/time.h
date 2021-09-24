/*
 *  time.h - time package
 *
 *  Created on: Sep 23, 2021
 *      Author: Tim Stark
 */

#pragma once

// typedef int64_t attoseconds_t;
// typedef int64_t seconds_t;

// GCC and Clang support __int128_t for 64-bit x86 processors
typedef __int128_t attoseconds_t;

// Time is attosecond-accurate timing system by using 128-bit integer word.
//
// second        = 1e0 seconds
// millisecond   = 1e-3 seconds
// microsecond   = 1e-6 seconds
// nanosecond    = 1e-9 seconds
// picosecond    = 1e-12 seconds
// femtosecond   = 1e-15 seconds
// attosecond    = 1e-18 seconds

// constexpr attoseconds_t ATTOSECONDS_PER_SECOND_SQRT = 1'000'000'000;
// constexpr attoseconds_t ATTOSECONDS_PER_SECOND = ATTOSECONDS_PER_SECOND_SQRT * ATTOSECONDS_PER_SECOND_SQRT;
constexpr attoseconds_t ATTOSECONDS_PER_SECOND = 1'000'000'000'000'000'000;
constexpr attoseconds_t ATTOSECONDS_PER_MILLISECOND = ATTOSECONDS_PER_SECOND / 1'000;
constexpr attoseconds_t ATTOSECONDS_PER_MICROSECOND = ATTOSECONDS_PER_SECOND / 1'000'000;
constexpr attoseconds_t ATTOSECONDS_PER_NANOSECOND = ATTOSECONDS_PER_SECOND / 1'000'000'000;

template <typename T>
inline constexpr attoseconds_t HZ_TO_ATTOSECONDS(T &&val) { return ATTOSECONDS_PER_SECOND / (val); }
// inline constexpr attoseconds_t HZ_TO_ATTOSECONDS(double &&val) { return double(ATTOSECONDS_PER_SECOND) / double(val); }

namespace emu::lib::util
{
    class Attotime
    {
    public:
        Attotime() = default;
        Attotime(int64_t attoseconds) : attoseconds(attoseconds) {}
        Attotime(int64_t seconds, int64_t attoseconds)
        : attoseconds(seconds * ATTOSECONDS_PER_SECOND + attoseconds)
        { }

        inline bool isZero() { return attoseconds == 0; }
        inline bool isNever() { return attoseconds == -1; }

        inline int64_t getSeconds() const { return attoseconds / ATTOSECONDS_PER_SECOND; }
        inline int64_t getAttoseconds() const { return attoseconds % ATTOSECONDS_PER_SECOND; }
        inline attoseconds_t getTime() const { return attoseconds; }

        static Attotime fromHz(uint32_t freq)
        {
            return (freq > 1) ? Attotime(1, HZ_TO_ATTOSECONDS(freq))
                : (freq == 1) ? Attotime(1, 0) : Attotime(-1);
        }

        attoseconds_t attoseconds = 0;   
    };
}

using attotime_t = emu::lib::util::Attotime;