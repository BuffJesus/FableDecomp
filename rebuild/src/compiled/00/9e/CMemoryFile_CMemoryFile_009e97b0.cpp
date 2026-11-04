#include "fable_memory_file.h"
CMemoryFile::CMemoryFile(const CWideString& pathname, FileAccessType accessType, unsigned long flags)
{
    Open(pathname, accessType, flags);
}
