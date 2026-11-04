// Behaviour of CDataOutputStream::WriteRepeatedByte(value, count) 0x00994230:
// writes `count` bytes equal to `value` through the stream's virtual Write
// (slot 4) in chunks of at most 256; nothing for count 0 (a negative count
// overruns the fill, as in retail).
#include "fable_data_output_stream.h"
#include <stdio.h>
#include <string.h>

static unsigned char g_out[1024];
static long g_len;
static int g_calls;
static long g_chunks[8];
static void* g_self;
static void FABLE_FASTCALL FakeWrite(void* self, void*, const void* data, long size)
{
    if (self != g_self) return;
    g_chunks[g_calls++ & 7] = size;
    memcpy(g_out + g_len, data, size);
    g_len += size;
}

static int Run(unsigned char value, long count, int wantCalls)
{
    memset(g_out, 0x5A, sizeof(g_out));
    g_len = 0; g_calls = 0;
    static void* vtable[5] = { 0, 0, 0, 0, (void*)&FakeWrite };
    static void* object[4] = { vtable };
    g_self = object;
    reinterpret_cast<CDataOutputStream*>(object)->WriteRepeatedByte(value, count);
    long want = count > 0 ? count : 0;
    if (g_len != want || g_calls != wantCalls) { printf("count %ld: len %ld calls %d\n", count, g_len, g_calls); return 1; }
    for (long i = 0; i < want; ++i) if (g_out[i] != value) { printf("byte %ld\n", i); return 1; }
    for (int c = 0; c + 1 < g_calls; ++c) if (g_chunks[c] != 256) { puts("chunk"); return 1; }
    return 0;
}

int main()
{
    if (Run(0, 4, 1) || Run(0xAB, 256, 1) || Run(0x11, 600, 3) || Run(0x77, 0, 0)) return 1;
    if (g_chunks[0] != 256) { puts("first chunk"); return 1; }
    Run(0x33, 513, 3);
    if (g_chunks[2] != 1) { puts("last chunk"); return 1; }
    puts("FABLETLC_CDATAOUTPUTSTREAM_WRITEREPEATEDBYTE_00994230_PASS");
    return 0;
}