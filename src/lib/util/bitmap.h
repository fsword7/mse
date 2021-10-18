/*
 *  bitmap.h - Bitmap package
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */


namespace emu::lib::util
{
    enum BitmapFormat
    {
        BitmapFormatInvalid = 0,
        BitmapFotmatIndex8,
        BitmapFormatIndex16,
        BitmapFormatIndex32,
        BitmapFormatIndex64,
        BitmapFormatRGB32,
        BitmapFormatRGBA32
    };

    class Rectangle
    {
        public:
            constexpr Rectangle() = default;
            constexpr Rectangle(int xmin, int xmax, int ymin, int ymax)
            : yMin(ymin), yMax(ymax), xMin(xmin), xMax(xmax)
            { }

            inline constexpr int getLeft() const    { return xMin; }
            inline constexpr int getRight() const   { return xMax; }
            inline constexpr int getTop() const     { return yMin; }
            inline constexpr int getBottom() const  { return yMax; }

            inline constexpr int getWidth() const   { return xMax + 1 - xMin; }
            inline constexpr int getHeight() const  { return yMax + 1 - yMin; }
            inline constexpr int getXCenter() const { return (xMin + xMax + 1) / 2; }
            inline constexpr int getYCenter() const { return (yMin + yMax + 1) / 2; }

            inline constexpr bool isEmpty() const   { return (yMin > yMax) || (xMin > xMax); }
            inline constexpr bool contains(int x, int y)
                { return (yMin >= y) && (y <= yMax) && (x >= xMin) && (x <= xMax); }
            inline constexpr bool contains(const Rectangle &rect)
                { return (yMin <= rect.yMin) && (yMax >= rect.yMax) &&
                         (xMin <= rect.xMin) && (xMax >= rect.xMax); }

            constexpr void set(int xmin, int xmax, int ymin, int ymax)
            { xMin = xmin; xMax = xmax; yMin = ymin; yMax = ymax; }

            int xMin = 0; // left coordinate    (minimum X)
            int xMax = 0; // right coordinate   (maximum X)
            int yMin = 0; // top coordinate     (minimum Y)
            int yMax = 0; // bottom coordinate  (maximum Y)
    };

    class Bitmap
    {
        public:
            Bitmap() = default;
            Bitmap(BitmapFormat format, int bpp, int width = 0, int height = 0);
            Bitmap(BitmapFormat format, int bpp, void *base, int width, int height);
            virtual ~Bitmap() = default;

            void allocate(int width, int height);
            void resize(int width, int height);

        private:
            bool hasValidFormat();
            int computeRowPixels(int width);

            BitmapFormat format;

            int width     = 0;
            int height    = 0;
            int bpp       = 0;
            int rowPixels = 0;

            offs_t dataSize = 0;
            void  *data = nullptr;
    };
    
    template <typename PixelType>
    class BitmapSpecific : public Bitmap
    {
            static constexpr int PIXEL_BITS = sizeof(PixelType) * 8;

        public:
            using pixel_t = PixelType;

            // BitmapSpecific(BitmapFormat format, int bpp, int width = 0, int height = 0);
            // BitmapSpecific(BitmapFormat format, int bpp, void *base, int width, int height);

    };

    // extern template class BitmapSpecific<uint8_t>;
    // extern template class BitmapSpecific<uint16_t>;
    // extern template class BitmapSpecific<uint32_t>;
    // extern template class BitmapSpecific<uint64_t>;
}

using rect_t = emu::lib::util::Rectangle;

// Generic data type definitions
using bitmap_t   = emu::lib::util::Bitmap;
using bitmap8_t  = emu::lib::util::BitmapSpecific<uint8_t>;
using bitmap16_t = emu::lib::util::BitmapSpecific<uint16_t>;
using bitmap32_t = emu::lib::util::BitmapSpecific<uint32_t>;
using bitmap64_t = emu::lib::util::BitmapSpecific<uint64_t>;