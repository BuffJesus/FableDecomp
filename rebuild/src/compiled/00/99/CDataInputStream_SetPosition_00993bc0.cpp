// CDataInputStream::SetPosition(unsigned long) @ 0x00993BC0 (FableWin
// ?SetPosition@CDataInputStream@@UAEXK@Z, vtable slot 1). A position inside
// the current source window just moves the window cursor; anything else drops
// the window, so the next read refills it from the new position.
#include "fable_data_input_stream.h"

void CDataInputStream::SetPosition(unsigned long position)
{
    if (CurrentSourcePtr != 0 && position >= SourceChunkStreamPos &&
        position <= StreamPos + SourceChunkBytesRemaining)
    {
        SourceChunkBytesRemaining = (StreamPos + SourceChunkBytesRemaining) - position;
        CurrentSourcePtr = (CurrentSourcePtr - StreamPos) + position;
        StreamPos = position;
    }
    else
    {
        CurrentSourcePtr = 0;
        SourceChunkStreamPos = 0;
        SourceChunkBytesRemaining = 0;
        StreamPos = position;
    }
}
