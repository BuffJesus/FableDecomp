// Behaviour fixture for the input-stream source hook at 0x00993B90 (called
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
    fable_u8 raw[sizeof(CDataInputStream)];
    memset(raw, 0xA5, sizeof(raw));
    CDataInputStream* s = reinterpret_cast<CDataInputStream*>(raw);
    const fable_u8* source = (const fable_u8*)0x1234;
    long size = 77;
    Access::NextChunk(s, source, size);
    bool ok = source == (const fable_u8*)0x1234 && size == 77 && raw[4] == 0xA5 && raw[0x17] == 0xA5;
    if (!ok)
    {
        printf("FABLETLC_STREAM_SOURCE_HOOKS_00993B90_PASS FAIL\n");
        return 1;
    }
    printf("FABLETLC_STREAM_SOURCE_HOOKS_00993B90_PASS\n");
    return 0;
}
