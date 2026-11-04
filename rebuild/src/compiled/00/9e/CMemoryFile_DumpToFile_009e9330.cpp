#include "fable_memory_file.h"
#include "fable_disk_file.h"
void CMemoryFile::DumpToFile()
{
    CDiskFileWin32 file(Pathname, AccessType, 2);
    file.Write(DataBuffer.begin(), DataBuffer.size(), 0);
}
