


#pragma once

namespace emu
{
    class ioFile
    {
        public:
        ioFile(uint64_t flags);
        virtual ~ioFile() = default;

        bool open(fs::path fname);

        void close();
    };
}