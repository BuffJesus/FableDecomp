#include "fable_memory_file.h"
#include "fable_file_exception.h"
void CMemoryFile::MovePosition(long offset)
{
    long position = Position + offset;
    if (position < 0)
    {
        CCharString message = CCharString("") + "Cannot move before beginning of file";
        throw CBBBFileException(message, CBBBFileException::BAD_SEEK);
    }
    if (position > Length)
    {
        CCharString message = CCharString("") + "Cannot seek past end of file";
        throw CBBBFileException(message, CBBBFileException::BAD_SEEK);
    }
    Position = position;
}
