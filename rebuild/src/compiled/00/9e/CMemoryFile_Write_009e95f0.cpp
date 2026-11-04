#include "fable_memory_file.h"
#include "fable_file_exception.h"
void CMemoryFile::Write(const void* data, long size, CASuspendableProcess*)
{
    const unsigned char* source = static_cast<const unsigned char*>(data);
    if (source == 0)
    {
        CCharString message = CCharString("") + "Null src parameter";
        throw CBBBFileException(message, CBBBFileException::BAD_PARAMETERS);
    }
    if (size != 0)
    {
        long end = Position + size;
        __int64 wideEnd = end;
        if (wideEnd >= 0 && wideEnd <= 0x7fffffff)
        {
            if (AccessType == FILE_ACCESS_READ)
            {
                CCharString message = CCharString("") + "File is not open for writing";
                throw CBBBFileException(message, CBBBFileException::NOT_OPEN_FOR_WRITING);
            }
            if (end > Length)
            {
                Length = end;
                DataBuffer.resize(end);
            }
            for (long i = size; i > 0; --i)
                DataBuffer[Position++] = *source++;
        }
    }
}
