// Behaviour fixture for the input-stream source hook at 0x009943B0 (called
// non-virtually on raw storage through the header's test-access friend).
#include <stdio.h>
#include <string.h>

#include "fable_data_input_stream.h"

class CDataInputStreamTestAccess
{
public:
    static bool FileNeeds(CFileDataInputStream* s, long size) { return s->CFileDataInputStream::NeedsBufferedRead(size); }
    static bool Needs(CDataInputStream* s, long size) { return s->CDataInputStream::NeedsBufferedRead(size); }
    static void NextChunk(CDataInputStream* s, const fable_u8*& source, long& size) { s->CDataInputStream::InternalReadNextSourceChunk(source, size); }
    static void Unbuffered(CDataInputStream* s, fable_u8* data, long size) { s->CDataInputStream::InternalReadUnbuffered(data, size); }
};
typedef CDataInputStreamTestAccess Access;

int main()
{
    fable_u8 raw[sizeof(CFileDataInputStream)];
    memset(raw, 0xA5, sizeof(raw));
    CFileDataInputStream* s = reinterpret_cast<CFileDataInputStream*>(raw);
    s->BufferSize = 0x4000;
    bool ok = Access::FileNeeds(s, 0x3FFF) && !Access::FileNeeds(s, 0x4000) &&
              !Access::FileNeeds(s, 0x10000) && Access::FileNeeds(s, -1);
    s->BufferSize = 0;
    ok = ok && !Access::FileNeeds(s, 0);
    if (!ok)
    {
        printf("FABLETLC_STREAM_SOURCE_HOOKS_009943B0_PASS FAIL\n");
        return 1;
    }
    printf("FABLETLC_STREAM_SOURCE_HOOKS_009943B0_PASS\n");
    return 0;
}
