// CDataInputStream::NeedsBufferedRead(long) @ 0x00993BA0 (Ego_r
// ?NeedsBufferedRead@CDataInputStream@@EAE_NJ@Z, vtable 0x129A6B0 slot 9): the
// base stream never reads through a buffer.
#include "fable_data_input_stream.h"

bool CDataInputStream::NeedsBufferedRead(long)
{
    return false;
}
