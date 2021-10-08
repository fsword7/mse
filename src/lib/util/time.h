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
typedef int64_t seconds_t;

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

constexpr attoseconds_t ATTOTIME_MAX_SECONDS = ATTOSECONDS_PER_SECOND * 1'000'000'000;
constexpr attoseconds_t ATTOTIME_ZERO = 0;
constexpr attoseconds_t ATTOTIME_NEVER = ATTOTIME_MAX_SECONDS;

template <typename T>
inline constexpr attoseconds_t HZ_TO_ATTOSECONDS(T &&val) { return ATTOSECONDS_PER_SECOND / (val); }
// inline constexpr attoseconds_t HZ_TO_ATTOSECONDS(double &&val) { return double(ATTOSECONDS_PER_SECOND) / double(val); }

template <typename T>
inline constexpr double ATTOSECCONDS_TO_HZ(T &&val) { return double(ATTOSECONDS_PER_SECOND) / double(val); }

namespace emu::lib::util
{
    class Attotime
    {
    public:
        Attotime() = default;
        Attotime(attoseconds_t attoseconds) : attoseconds(attoseconds) {}
        // Attotime(int64_t attoseconds) : attoseconds(attoseconds) {}
        Attotime(int64_t seconds, int64_t attoseconds)
        : attoseconds(seconds * ATTOSECONDS_PER_SECOND + attoseconds)
        { }

        static Attotime zero;
        static Attotime never;

        inline bool isZero() { return attoseconds == 0; }
        inline bool isNever() { return attoseconds == ATTOTIME_NEVER; }

        inline int64_t getSeconds() const { return attoseconds / ATTOSECONDS_PER_SECOND; }
        inline int64_t getAttoseconds() const { return attoseconds % ATTOSECONDS_PER_SECOND; }
        inline attoseconds_t getTime() const { return attoseconds; }

        inline attoseconds_t getAsNanoseconds() const { return attoseconds / ATTOSECONDS_PER_NANOSECOND; }

        inline double getAsHertz() const { return double(ATTOSECONDS_PER_SECOND) / double (attoseconds); }

        static Attotime fromHz(uint32_t freq)
        {
            return (freq > 1) ? Attotime(0, HZ_TO_ATTOSECONDS(freq))
                : (freq == 1) ? Attotime(1, 0) : never;
        }

        // attotime_t &operator += (const attotime_t &right) noexcept;
        // attotime_t &operator -= (const attotime_t &right) noexcept;
        // attotime_t &operator *= (uint64_t factor);
        // attotime_t &operator /= (uint64_t factor);

        inline Attotime &operator += (const Attotime &right) noexcept
        {
            // Return as never result.
            if (this->attoseconds >= ATTOTIME_MAX_SECONDS || right.attoseconds >= ATTOTIME_MAX_SECONDS)
                return *this = never;

            this->attoseconds = this->attoseconds + right.attoseconds;
            if (this->attoseconds >= ATTOTIME_MAX_SECONDS)
                return *this = never;
            return *this;  
        }

        inline Attotime &operator -= (const Attotime &right) noexcept
        {
            // Return as never result.
            if (this->attoseconds >= ATTOTIME_MAX_SECONDS)
                return *this = never;

            this->attoseconds = this->attoseconds - right.attoseconds;
            return *this;   
        }

        inline Attotime &operator *= (uint64_t factor) noexcept
        {
            if (this->attoseconds >= ATTOTIME_MAX_SECONDS)
                return *this = never;

            this->attoseconds = this->attoseconds * factor;
            return *this;
        }

        inline Attotime &operator /= (uint64_t factor) noexcept
        {
            if (this->attoseconds >= ATTOTIME_MAX_SECONDS)
                return *this = never;
            if (factor == 0)
                return *this;
            this->attoseconds = this->attoseconds / factor;
            return *this;
        }
        
        attoseconds_t attoseconds = 0;

    };
}

using attotime_t = emu::lib::util::Attotime;


inline attotime_t operator + (const attotime_t &left, const attotime_t &right) noexcept
{
    attotime_t result;

    // Return as never result.
    if (left.attoseconds >= ATTOTIME_MAX_SECONDS || right.attoseconds >= ATTOTIME_MAX_SECONDS)
        return attotime_t::never;

    result.attoseconds = left.attoseconds + right.attoseconds;
    if (result.attoseconds >= ATTOTIME_NEVER)
        return attotime_t::never;
    return result;   
}

inline attotime_t operator - (const attotime_t &left, const attotime_t &right) noexcept
{
    attotime_t result;

    // Return as never result.
    if (left.attoseconds >= ATTOTIME_MAX_SECONDS)
        return attotime_t::never;

    result.attoseconds = left.attoseconds - right.attoseconds;
    return result;   
}

inline attotime_t operator * (const attotime_t &left, uint64_t factor)
{
    attotime_t result;

    if (left.attoseconds >= ATTOTIME_MAX_SECONDS)
        return attotime_t::never;
    result.attoseconds = left.attoseconds * factor;
    return result;
}

inline attotime_t operator * (uint64_t factor, const attotime_t &right)
{
    attotime_t result;

    if (right.attoseconds >= ATTOTIME_MAX_SECONDS)
        return attotime_t::never;
    result.attoseconds = right.attoseconds * factor;
    return result;
}

inline attotime_t operator / (const attotime_t &left, uint64_t factor)
{
    attotime_t result;

    if (left.attoseconds >= ATTOTIME_MAX_SECONDS)
        return attotime_t::never;
    if (factor == 0)
        return left;
    result.attoseconds = left.attoseconds * factor;
    return result;
}


inline constexpr bool operator == (const attotime_t &left, const attotime_t &right) noexcept
{
    return (left.attoseconds == right.attoseconds);
}

inline constexpr bool operator != (const attotime_t &left, const attotime_t &right) noexcept
{
    return (left.attoseconds != right.attoseconds);
}

inline constexpr bool operator < (const attotime_t &left, const attotime_t &right) noexcept
{
    return (left.attoseconds < right.attoseconds);
}

inline constexpr bool operator <= (const attotime_t &left, const attotime_t &right) noexcept
{
    return (left.attoseconds <= right.attoseconds);
}

inline constexpr bool operator > (const attotime_t &left, const attotime_t &right) noexcept
{
    return (left.attoseconds > right.attoseconds);
}

inline constexpr bool operator >= (const attotime_t &left, const attotime_t &right) noexcept
{
    return (left.attoseconds >= right.attoseconds);
}