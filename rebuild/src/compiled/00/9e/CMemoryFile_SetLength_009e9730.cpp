#include "fable_memory_file.h"
#include "fable_file_exception.h"
void CMemoryFile::SetLength(long length)
{
    if (length > Length)
    {
        Length = length;
        DataBuffer.resize(length);
    }
}
