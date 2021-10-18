/*
 *  color.h - color/palette package
 *
 *  Created on: Oct 17, 2021
 *      Author: Tim Stark
 */

#pragma once

class Color
{
    public:
        constexpr Color() = default;

        constexpr Color(uint32_t rgba) : data(rgba) {}

        constexpr Color(uint8_t r, uint8_t g, uint8_t b)
        : data((255 << 24) | (r << 16) | (g << 8) | b)
        {}

        constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        : data((a << 24) | (r << 16) | (g << 8) | b)
        {}

        // Getters
        constexpr uint8_t getAlpha() const { return data >> 24; }
        constexpr uint8_t getRed() const   { return data >> 16; }
        constexpr uint8_t getGreen() const { return data >> 8; }
        constexpr uint8_t getBlue() const  { return data; }

        // Colors for monochrome monitors
        static constexpr Color black() { return Color(0, 0, 0); }
        static constexpr Color white() { return Color(255, 255, 255); }
        static constexpr Color green() { return Color(0, 255, 0); }
        static constexpr Color amber() { return Color(255, 191, 0); }

        static constexpr Color transparent() { return Color(0, 0, 0, 0); }

    private:
        uint32_t data = 0;
};
