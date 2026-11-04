// Behaviour fixture for CDefStringTable::ReadTable (0x009D4AB0): a stream with
// its whole content in the window (header word, RandomID, count, size, text).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_def_string.h"
#include "fable_data_input_stream.h"
#include "test_support/base_class_stubs.h"
#include "test_support/data_input_stream_stubs.h"

const char* g_DefStringTableText_013CA824;

namespace
{
    int g_reserves, g_copies, g_peeks;
    unsigned long g_reserveSize, g_copySize;
    const void* g_reserveThis;
    const void* g_copyThis;
    CDataInputStream* g_copyStream;
    unsigned long g_copyPos;
    char g_data[16] = "\x10\x00\x00\x00" "ALPHA";
}

CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CDataInputStream::CDataInputStream()
    : StreamPos(0), StreamSize(0), CurrentSourcePtr(0), SourceChunkStreamPos(0), SourceChunkBytesRemaining(0)
{
}
void CDataInputStream::SetPosition(unsigned long position) { StreamPos = position; }
void CDataInputStream::ReadWithSrcChunkOverflow(void*, long) { abort(); }

void CMemoryDataOutputStream::ReserveSize(unsigned long size)
{
    ++g_reserves;
    g_reserveThis = this;
    g_reserveSize = size;
}
void CDataOutputStream::CopyFrom(CDataInputStream& stream, unsigned long size)
{
    ++g_copies;
    g_copyThis = this;
    g_copyStream = &stream;
    g_copySize = size;
    g_copyPos = stream.StreamPos;
}
const void* CMemoryDataOutputStream::PeekData() const
{
    ++g_peeks;
    return g_data;
}

int main()
{
    const unsigned long words[4] = { 0xDEADBEEF, 0x12345678, 42, 9 };
    fable_u8 source[16];
    memcpy(source, words, sizeof(words));

    CDataInputStream stream;
    stream.StreamSize = 16;
    stream.CurrentSourcePtr = source;
    stream.SourceChunkBytesRemaining = 16;

    fable_u8 raw[sizeof(CDefStringTable)];
    memset(raw, 0xA5, sizeof(raw));
    CDefStringTable& table = *reinterpret_cast<CDefStringTable*>(raw);
    table.ReadTable(stream);

    const bool passed =
        table.RandomID == 0x12345678 && table.StringCount == 42 && table.SharedCount == 0xA5A5A5A5 &&
        stream.StreamPos == 16 && stream.SourceChunkBytesRemaining == 0 &&
        g_reserves == 1 && g_reserveThis == &table.StringStream && g_reserveSize == 9 &&
        g_copies == 1 && g_copyThis == &table.StringStream && g_copyStream == &stream && g_copySize == 9 &&
        g_copyPos == 16 && g_peeks == 1 && g_DefStringTableText_013CA824 == g_data + 4;
    if (!passed)
    {
        printf("FABLETLC_CDEFSTRINGTABLE_READTABLE_009D4AB0 FAIL\n");
        return 1;
    }
    printf("FABLETLC_CDEFSTRINGTABLE_READTABLE_009D4AB0_PASS\n");
    return 0;
}