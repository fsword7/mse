


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

        int read(uint8_t *data, int length);
        int write(uint8_t *data, int length);

    private:
        fstream file;
    };
}