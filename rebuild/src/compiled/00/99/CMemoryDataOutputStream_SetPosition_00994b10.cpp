// CMemoryDataOutputStream::SetPosition(unsigned long) @ 0x00994B10 (FableWin
// ?SetPosition@CMemoryDataOutputStream@@UAEXK@Z; vtable 0x0129A788 slot 1).
#include "fable_data_output_stream.h"
#include "fable_file_exception.h"

// Seek within what has been written; past the end is a bad seek.
void CMemoryDataOutputStream::SetPosition(unsigned long position)
{
    if (position < 0 || position > Data.size())
        throw CBBBFileException(CBBBFileException::BAD_SEEK);
    CurrentPosition = position;
}
