// Behaviour fixture for CDataInputStream::ReadCompressed 0x00995FB0. The
// stream holds a 16-bit size (0xFFFF: a 32-bit size follows), then either
// that many LZO bytes plus three raw trailing bytes, or (size <= 0) the data
// itself. The chunk-overflow read, the vector(n) allocation and the LZO
// decompressor are fixture-side: the "decompressor" here byte-reverses its
// input into the destination and reports a chosen output length, so a wrong
// length must reach CAFile's error callback.
#include <new>
#include "fable_data_input_stream.h"
#include "fable_carray.h"
#include "fable_filesystem.h"
#include "fable_lzo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char g_source[256];
static unsigned g_sourceLength, g_overflowCalls, g_lzoCalls, g_callbacks;
static unsigned long g_reportedLength;
void CDataInputStream::ReadWithSrcChunkOverflow(void* out, long size)
{
    // Like the real routine, this drains the current chunk; later reads overflow too.
    memcpy(out, g_source + StreamPos, size);
    StreamPos += size;
    CurrentSourcePtr = g_source + StreamPos;
    SourceChunkBytesRemaining = 0;
    ++g_overflowCalls;
}
namespace _STL
{
    template <> vector<unsigned char>::vector(size_t n)
        : _Vector_base<unsigned char, allocator<unsigned char> >(allocator<unsigned char>())
    {
        _M_start = static_cast<unsigned char*>(malloc(n ? n : 1));
        memset(_M_start, 0, n);
        _M_finish = _M_start + n;
        _M_end_of_storage._M_data = _M_finish;
    }
}
extern "C" int lzo1x_decompress_asm_fast(const unsigned char* src, unsigned long srcLength,
                                         unsigned char* dst, unsigned long* dstLength, void* work)
{
    ++g_lzoCalls;
    if (work != 0) return -1;
    const unsigned long capacity = *dstLength;
    for (unsigned long i = 0; i < srcLength && i < capacity; ++i) dst[i] = src[srcLength - 1 - i];
    *dstLength = g_reportedLength;
    return 0;
}
void (FABLE_FASTCALL* CAFile::m_errorCallback)() = 0;
static void FABLE_FASTCALL OnError() { ++g_callbacks; }

static int g_fail;
#define CHECK(c, code) do { if (!(c)) { printf("READ_COMPRESSED_FAIL %d line %d\n", code, __LINE__); g_fail = 1; } } while (0)

static CDataInputStream* Stream(unsigned char* raw, unsigned available)
{
    memset(raw, 0xa5, sizeof(CDataInputStream) + 8);
    CDataInputStream* s = reinterpret_cast<CDataInputStream*>(raw + 4);
    s->StreamPos = 0;
    s->StreamSize = g_sourceLength;
    s->CurrentSourcePtr = g_source;
    s->SourceChunkStreamPos = 0;
    s->SourceChunkBytesRemaining = available < g_sourceLength ? available : g_sourceLength;
    return s;
}

int main()
{
    int cases = 0;
    unsigned char raw[sizeof(CDataInputStream) + 8];
    static const long sizes[] = { 5, 9, 20 };
    for (int k = 0; k < 3; ++k) for (int wide = 0; wide < 2; ++wide) for (int good = 0; good < 2; ++good)
        for (int callback = 0; callback < 2; ++callback) for (unsigned available = 0; available < 64; available += 9)
    {
        // Compressed block of n bytes, then three raw trailing bytes.
        const long size = sizes[k];
        const unsigned n = (unsigned)size - 3 + k;
        unsigned pos = 0;
        if (wide) { g_source[pos++] = 0xff; g_source[pos++] = 0xff; memcpy(g_source + pos, &n, 4); pos += 4; }
        else { g_source[pos++] = (unsigned char)n; g_source[pos++] = 0; }
        const unsigned block = pos;
        for (unsigned i = 0; i < n; ++i) g_source[pos++] = (unsigned char)(0x40 + i * 7);
        g_source[pos++] = 0xd1; g_source[pos++] = 0xd2; g_source[pos++] = 0xd3;
        g_sourceLength = pos;
        g_reportedLength = good ? size - 3 : size - 2;
        CAFile::m_errorCallback = callback ? &OnError : 0;
        g_overflowCalls = g_lzoCalls = g_callbacks = 0;
        unsigned char out[64];
        memset(out, 0xee, sizeof(out));
        CDataInputStream* s = Stream(raw, available);
        s->CDataInputStream::ReadCompressed(out, size);
        CHECK(g_lzoCalls == 1 && s->StreamPos == g_sourceLength, 1);
        for (long i = 0; i < size - 3; ++i) CHECK(out[i] == g_source[block + n - 1 - i], 2);
        CHECK(out[size - 3] == 0xd1 && out[size - 2] == 0xd2 && out[size - 1] == 0xd3 && out[size] == 0xee, 3);
        CHECK(g_callbacks == (unsigned)(callback && !good), 4);
        CHECK(raw[3] == 0xa5 && raw[sizeof(raw) - 4] == 0xa5, 5);
        ++cases;
    }
    // Stored data: a size of zero (or negative) means the bytes follow as they are.
    for (int marker = 0; marker < 2; ++marker) for (unsigned available = 0; available < 40; available += 7)
    {
        unsigned pos = 0;
        if (marker) { g_source[pos++] = 0xff; g_source[pos++] = 0xff; const long zero = -5; memcpy(g_source + pos, &zero, 4); pos += 4; }
        else { g_source[pos++] = 0; g_source[pos++] = 0; }
        const unsigned start = pos;
        for (unsigned i = 0; i < 12; ++i) g_source[pos++] = (unsigned char)(0x90 + i);
        g_sourceLength = pos;
        g_lzoCalls = 0;
        unsigned char out[16];
        memset(out, 0xee, sizeof(out));
        CDataInputStream* s = Stream(raw, available);
        s->CDataInputStream::ReadCompressed(out, 12);
        CHECK(g_lzoCalls == 0 && !memcmp(out, g_source + start, 12) && out[12] == 0xee && s->StreamPos == g_sourceLength, 6);
        ++cases;
    }
    // Nothing to read.
    for (long size = -2; size <= 0; ++size)
    {
        g_sourceLength = 8;
        CDataInputStream* s = Stream(raw, 8);
        s->CDataInputStream::ReadCompressed(raw, size);
        CHECK(s->StreamPos == 0 && s->SourceChunkBytesRemaining == 8, 7);
        ++cases;
    }
    if (g_fail) return 1;
    printf("READ_COMPRESSED_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
