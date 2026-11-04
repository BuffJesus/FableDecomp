#include "fable_memory_file.h"
#include "fable_file_exception.h"
void CMemoryFile::Open()
{
    Pathname = L"";
    Position = 0;
    Length = 0;
    Opened = true;
}
