#include "fable_memory_file.h"
#include "fable_file_exception.h"
void CMemoryFile::Read(void* data, long size, CASuspendableProcess*)
{
    if (data == 0)
    {
        CCharString message = CCharString("") + "Null dst parameter";
        throw CBBBFileException(message, CBBBFileException::BAD_PARAMETERS);
    }
    if (size != 0)
    {
        if (Position + size > Length)
        {
            CCharString message = CCharString("") + "Cannot read past end of file";
            throw CBBBFileException(message, CBBBFileException::END_OF_FILE);
        }
        unsigned char* destination = static_cast<unsigned char*>(data);
        for (long i = size; i > 0; --i)
            *destination++ = DataBuffer[Position++];
    }
}
