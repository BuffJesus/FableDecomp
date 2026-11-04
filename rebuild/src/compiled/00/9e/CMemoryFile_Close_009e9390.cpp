#include "fable_memory_file.h"
#include "fable_file_exception.h"
void CMemoryFile::Close()
{
    Opened = false;
    DataBuffer.clear();
}
