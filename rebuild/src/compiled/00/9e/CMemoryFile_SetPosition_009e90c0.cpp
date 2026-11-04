#include "fable_memory_file.h"
#include "fable_file_exception.h"
void CMemoryFile::SetPosition(long position)
{
    if (position > Length)
    {
        CCharString message = CCharString("") + "Cannot seek past end of file";
        throw CBBBFileException(message, CBBBFileException::BAD_SEEK);
    }
    Position = position;
}
