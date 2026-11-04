#include "fable_memory_file.h"
#include <string.h>
void CMemoryFile::Open(void* data, long size)
{
    Pathname = L"";
    Position = 0;
    Length = size;
    Opened = true;
    DataBuffer.resize(size);
    memcpy(DataBuffer.GetAsCArray(), data, size);
}
