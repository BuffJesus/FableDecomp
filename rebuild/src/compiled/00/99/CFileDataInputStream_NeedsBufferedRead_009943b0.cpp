// CFileDataInputStream::NeedsBufferedRead(long) @ 0x009943B0 (Ego_r
// ?NeedsBufferedRead@CFileDataInputStream@@EAE_NJ@Z, vtable 0x129A728 slot 9):
// reads smaller than the buffer go through the buffered window.
#include "fable_data_input_stream.h"

bool CFileDataInputStream::NeedsBufferedRead(long size)
{
    return size < BufferSize;
}
