// Behaviour fixture for CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0):
// a test stream serves a 64-byte source in 3-byte chunks, or directly when
// the stream says it does not want a buffered read for the size.
#include <stdio.h>
#include <string.h>

#include "fable_data_input_stream.h"
#include "test_support/base_class_stubs.h"
#include "test_support/data_input_stream_stubs.h"

CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CDataInputStream::CDataInputStream()
    : StreamPos(0), StreamSize(0), CurrentSourcePtr(0), SourceChunkStreamPos(0), SourceChunkBytesRemaining(0)
{
}
void CDataInputStream::SetPosition(unsigned long position) { StreamPos = position; }
CFileDataInputStream::~CFileDataInputStream() {}

namespace
{
    fable_u8 g_source[64];
    int g_chunks;
    int g_unbuffered;
    long g_threshold;
}

class TestStream : public CDataInputStream
{
private:
    virtual void InternalReadNextSourceChunk(const fable_u8*& source, long& size)
    {
        ++g_chunks;
        source = g_source + SourceChunkStreamPos;
        size = 3;
    }
    virtual bool NeedsBufferedRead(long size) { return size < g_threshold; }
    virtual void InternalReadUnbuffered(fable_u8* data, long size)
    {
        ++g_unbuffered;
        memcpy(data, g_source + StreamPos, size);
    }
};

namespace
{
    bool Expect(const fable_u8* got, unsigned long from, long size)
    {
        return memcmp(got, g_source + from, size) == 0;
    }
}

int main()
{
    for (int i = 0; i < 64; ++i)
        g_source[i] = (fable_u8)(i * 7 + 1);

    int fail = 0;
    fable_u8 out[32];

    // A window with 2 bytes left at position 10; a buffered 7-byte read drains
    // them, then takes 3-byte chunks from 12 and 15.
    TestStream s;
    s.StreamPos = 10;
    s.StreamSize = 64;
    s.CurrentSourcePtr = g_source + 10;
    s.SourceChunkStreamPos = 9;
    s.SourceChunkBytesRemaining = 2;
    g_threshold = 100; g_chunks = 0; g_unbuffered = 0;
    memset(out, 0, sizeof(out));
    s.ReadWithSrcChunkOverflow(out, 7);
    if (!Expect(out, 10, 7) || s.StreamPos != 17 || g_chunks != 2 || g_unbuffered != 0 ||
        s.SourceChunkStreamPos != 15 || s.SourceChunkBytesRemaining != 1 || s.CurrentSourcePtr != g_source + 17)
        fail |= 1;

    // Unbuffered: drain the 1 byte left, read the rest directly, drop the window.
    g_threshold = 0; g_chunks = 0;
    memset(out, 0, sizeof(out));
    s.ReadWithSrcChunkOverflow(out, 9);
    if (!Expect(out, 17, 9) || s.StreamPos != 26 || g_chunks != 0 || g_unbuffered != 1 ||
        s.SourceChunkBytesRemaining != 0 || s.CurrentSourcePtr != 0 || s.SourceChunkStreamPos != 0)
        fail |= 2;

    // No window at all, buffered: chunks from the current position.
    g_threshold = 100; g_chunks = 0; g_unbuffered = 0;
    memset(out, 0, sizeof(out));
    s.ReadWithSrcChunkOverflow(out, 4);
    if (!Expect(out, 26, 4) || s.StreamPos != 30 || g_chunks != 2 || s.SourceChunkBytesRemaining != 2)
        fail |= 4;

    if (fail == 0)
        printf("FABLETLC_CDATAINPUTSTREAM_READWITHSRCCHUNKOVERFLOW_00993CA0_PASS\n");
    else
        printf("FABLETLC_CDATAINPUTSTREAM_READWITHSRCCHUNKOVERFLOW_00993CA0 FAIL %d\n", fail);
    return fail;
}
