

// address space namespace
namespace aspace2
{
    // Data width size definitions for handler function calls
    //
    // 0 = 8-bit data width
    // 1 = 16-bit data width
    // 2 = 32-bit data width
    // 3 = 64-bit data width

    template <int dWidth> struct HandlerSize {};
    template <> struct HandlerSize<0> { using uintx_t = uint8_t; };
    template <> struct HandlerSize<1> { using uintx_t = uint16_t; }
    template <> struct HandlerSize<2> { using uintx_t = uint32_t; }
    template <> struct HandlerSize<3> { using uintx_t = uint64_t; }

    // Address space type definitions
    enum spaceType
    {
        asProgram = 0,
        asData    = 1,
        asIOPort  = 2
    };


    class AddressSpaceConfig
    {

    };

    template <int Level, int dWidth, int aShift, endian_t eType>
    class MemoryAccessSpecific
    {
        using uintx_t = typename HandlerSize<dWidth>::uintx_t;




    };


    class AddressSpace
    {
        public:
            // Read/write access type definitions
            enum accessType { accRead, accWrite, accReadWrit };


            // Prepare for address map list
            void prepare(Console *user)
            {

            }

            // Populate memory space from map list
            void populate(Console *user)
            {

            }
    };
}