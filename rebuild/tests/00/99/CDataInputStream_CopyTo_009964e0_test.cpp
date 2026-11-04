// Behaviour fixture for CDataInputStream::CopyTo (0x009964E0): 0x10005 bytes
// copy as a 64 KiB chunk and a 5-byte chunk through one buffer; nothing is
// written for a zero size.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_data_input_stream.h"
#include "fable_data_output_stream.h"
#include "test_support/base_class_stubs.h"
#include "test_support/data_input_stream_stubs.h"

namespace
{
    const unsigned long kTotal = 0x10005;
    fable_u8* g_source;
    fable_u8* g_copied;
    unsigned long g_copiedBytes;
    int g_writes;
    long g_sizes[4];
    const void* g_buffers[4];
}

CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CDataInputStream::CDataInputStream()
    : StreamPos(0), StreamSize(0), CurrentSourcePtr(0), SourceChunkStreamPos(0), SourceChunkBytesRemaining(0)
{
}
void CDataInputStream::SetPosition(unsigned long position) { StreamPos = position; }
void CDataInputStream::ReadWithSrcChunkOverflow(void*, long) { abort(); }
CCharString::CCharString(const char*, long) : storage_(0) {}
CCharString::~CCharString() {}


struct TestOutput : CDataOutputStream
{
    virtual void SetPosition(unsigned long) {}
    virtual unsigned long GetPosition() const { return 0; }
    virtual void Close() {}
    virtual void Write(const void* data, long size)
    {
        if (g_writes < 4)
        {
            g_sizes[g_writes] = size;
            g_buffers[g_writes] = data;
        }
        ++g_writes;
        memcpy(g_copied + g_copiedBytes, data, size);
        g_copiedBytes += size;
    }
};

int main()
{
    g_source = (fable_u8*)malloc(kTotal);
    g_copied = (fable_u8*)malloc(kTotal);
    for (unsigned long i = 0; i < kTotal; ++i)
        g_source[i] = (fable_u8)(i * 13 + 7);

    CDataInputStream in;
    in.StreamSize = kTotal;
    in.CurrentSourcePtr = g_source;
    in.SourceChunkBytesRemaining = kTotal;
    TestOutput out;

    int fail = 0;
    in.CopyTo(out, 0);
    if (g_writes != 0 || in.StreamPos != 0) fail |= 1;

    in.CopyTo(out, kTotal);
    if (g_writes != 2 || g_sizes[0] != 0x10000 || g_sizes[1] != 5 || g_buffers[0] != g_buffers[1] ||
        g_copiedBytes != kTotal || memcmp(g_copied, g_source, kTotal) != 0 || in.StreamPos != kTotal)
        fail |= 2;

    if (fail == 0)
        printf("FABLETLC_CDATAINPUTSTREAM_COPYTO_009964E0_PASS\n");
    else
        printf("FABLETLC_CDATAINPUTSTREAM_COPYTO_009964E0 FAIL %d\n", fail);
    return fail;
}