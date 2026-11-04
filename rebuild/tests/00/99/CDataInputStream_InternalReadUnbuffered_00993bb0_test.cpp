// Behaviour fixture for the input-stream source hook at 0x00993BB0 (called
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
    fable_u8 data[8];
    memset(data, 0x5A, sizeof(data));
    Access::Unbuffered(s, data, 8);
    bool ok = data[0] == 0x5A && data[7] == 0x5A && raw[4] == 0xA5 && raw[0x17] == 0xA5;
    if (!ok)
    {
        printf("FABLETLC_STREAM_SOURCE_HOOKS_00993BB0_PASS FAIL\n");
        return 1;
    }
    printf("FABLETLC_STREAM_SOURCE_HOOKS_00993BB0_PASS\n");
    return 0;
}
