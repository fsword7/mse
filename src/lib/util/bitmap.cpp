/*
 *  bitmap.h - Bitmap package
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "lib/util/bitmap.h"

using namespace emu::lib::util;

Bitmap::Bitmap(BitmapFormat format, int bpp, int width, int height)
: format(format), bpp(bpp)
{
    assert(hasValidFormat());
    allocate(width, height);
}

Bitmap::Bitmap(BitmapFormat format, int bpp, void *base, int width, int height)
: format(format), bpp(bpp)
{
    assert(hasValidFormat());
}

// Validate bitmap formt and bits per pixel
bool Bitmap::hasValidFormat()
{
    switch (format)
    {
        case BitmapFormatInvalid:
            return false;
        
        case BitmapFormatIndex16:
            return bpp == 16;

        case BitmapFormatIndex32:
        case BitmapFormatRGB32:
        case BitmapFormatRGBA32:
            return bpp == 32;

        case BitmapFormatIndex64:
            return bpp == 64;
    }

    return false;
}

inline int Bitmap::computeRowPixels(int width)
{
    return width;
}

void Bitmap::allocate(int width, int height)
{
    // Check valid values first
    assert(format != BitmapFormatInvalid);
    assert(bpp == 8 || bpp == 16 || bpp == 32 || bpp == 64);

    // If width or height is zero or negative,
    // do nothing and return
    if (width <= 0 || height <= 0)
        return;

    rowPixels = computeRowPixels(width);
    this->width = width;
    this->height = height;

    dataSize = rowPixels * height * bpp / 8;
    data = new uint8_t[dataSize];

    memset(data, 0, dataSize);
}

void Bitmap::resize(int width, int height)
{

}

template class BitmapSpecific<uint8_t>;
template class BitmapSpecific<uint16_t>;
template class BitmapSpecific<uint32_t>;
template class BitmapSpecific<uint64_t>;
