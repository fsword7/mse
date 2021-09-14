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

// Generic data type definitions
using bitmap_t   = emu::lib::util::Bitmap;
using bitmap8_t  = emu::lib::util::BitmapSpecific<uint8_t>;
using bitmap16_t = emu::lib::util::BitmapSpecific<uint16_t>;
using bitmap32_t = emu::lib::util::BitmapSpecific<uint32_t>;
using bitmap64_t = emu::lib::util::BitmapSpecific<uint64_t>;