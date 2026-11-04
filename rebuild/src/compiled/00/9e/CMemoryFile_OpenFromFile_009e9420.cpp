#include "fable_memory_file.h"
void CMemoryFile::OpenFromFile(CAFile& file, long size, CASuspendableProcess* process)
{
    Pathname = file.GetPathname();
    Position = 0;
    Length = size;
    Opened = true;
    // The buffer is rounded up to whole units of the source's alignment.
    unsigned long alignment = file.GetAlignment();
    DataBuffer.resize((alignment + size - 1) / alignment * alignment);
    file.Read(DataBuffer.begin(), size, process);
}
