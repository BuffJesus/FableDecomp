// Behaviour fixture for CDataInputStream::SetPosition (0x00993BC0), called
// non-virtually on raw storage laid out as CDataInputStream.
#include <stdio.h>
#include <string.h>

#include "fable_data_input_stream.h"

namespace
{
    fable_u8 g_window[64];

    CDataInputStream* Stream(unsigned long* words, fable_u8* ptr, unsigned long pos,
                             unsigned long chunkPos, long remaining)
    {
        memset(words, 0xA5, 6 * sizeof(unsigned long));
        CDataInputStream* s = reinterpret_cast<CDataInputStream*>(words);
        s->StreamPos = pos;
        s->StreamSize = 100;
        s->CurrentSourcePtr = ptr;
        s->SourceChunkStreamPos = chunkPos;
        s->SourceChunkBytesRemaining = remaining;
        return s;
    }
}

int main()
{
    int fail = 0;
    unsigned long words[6];

    // Window [10, 30): cursor at 14 (ptr g_window+4), 16 bytes left.
    CDataInputStream* s = Stream(words, g_window + 4, 14, 10, 16);
    s->CDataInputStream::SetPosition(20);
    if (s->StreamPos != 20 || s->CurrentSourcePtr != g_window + 10 || s->SourceChunkBytesRemaining != 10 ||
        s->SourceChunkStreamPos != 10 || s->StreamSize != 100 || words[0] != 0xA5A5A5A5)
        fail |= 1;

    // Both window edges stay inside.
    s = Stream(words, g_window + 4, 14, 10, 16);
    s->CDataInputStream::SetPosition(10);
    if (s->CurrentSourcePtr != g_window || s->SourceChunkBytesRemaining != 20) fail |= 2;
    s = Stream(words, g_window + 4, 14, 10, 16);
    s->CDataInputStream::SetPosition(30);
    if (s->CurrentSourcePtr != g_window + 20 || s->SourceChunkBytesRemaining != 0) fail |= 4;

    // Outside the window (before and after): the window is dropped.
    s = Stream(words, g_window + 4, 14, 10, 16);
    s->CDataInputStream::SetPosition(9);
    if (s->StreamPos != 9 || s->CurrentSourcePtr != 0 || s->SourceChunkStreamPos != 0 ||
        s->SourceChunkBytesRemaining != 0 || s->StreamSize != 100)
        fail |= 8;
    s = Stream(words, g_window + 4, 14, 10, 16);
    s->CDataInputStream::SetPosition(31);
    if (s->StreamPos != 31 || s->CurrentSourcePtr != 0) fail |= 16;

    // No window at all.
    s = Stream(words, 0, 14, 10, 16);
    s->CDataInputStream::SetPosition(20);
    if (s->StreamPos != 20 || s->SourceChunkStreamPos != 0 || s->SourceChunkBytesRemaining != 0) fail |= 32;

    if (fail == 0)
        printf("FABLETLC_CDATAINPUTSTREAM_SETPOSITION_00993BC0_PASS\n");
    else
        printf("FABLETLC_CDATAINPUTSTREAM_SETPOSITION_00993BC0 FAIL %d\n", fail);
    return fail;
}
