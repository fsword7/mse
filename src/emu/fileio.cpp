
#include "emu/core.h"
#include "emu/fileio.h"

using namespace emu;

ioFile::ioFile(uint64_t flags)
{
}

bool ioFile::open(fs::path fname)
{
    file.open(fname, ios::in|ios::binary);

    return file.is_open();
}

void ioFile::close()
{
    file.close();
}

int ioFile::read(uint8_t *data, int length)
{
    file.read((char *)data, length);
    return file.gcount();
}

int ioFile::write(uint8_t *data, int length)
{
    file.write((char *)data, length);
    return file.gcount();
}