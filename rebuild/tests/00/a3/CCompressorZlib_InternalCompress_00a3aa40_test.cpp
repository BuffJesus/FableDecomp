#include "engine/CCompressorZlib.h"
#include <stdio.h>
#include <string.h>

static int g_init, g_feed, g_finish, g_end, g_error;
static const unsigned char* g_input;
static unsigned char* g_output;
static unsigned long g_size, g_capacity;

extern "C" int __cdecl z_deflateInit_(z_stream_s* stream, int level, const char* version, int bytes)
{
    ++g_init;
    if (level != 1 || strcmp(version, "1.2.1") || bytes != sizeof(z_stream_s) ||
        !stream->zalloc || !stream->zfree || stream->opaque) ++g_error;
    stream->total_in = 0;
    stream->total_out = 0;
    return 0;
}
extern "C" int __cdecl z_deflate(z_stream_s* stream, int flush)
{
    if (stream->next_in < g_input || stream->next_in > g_input + g_size ||
        stream->next_out < g_output || stream->next_out > g_output + g_capacity ||
        stream->avail_out != 1) ++g_error;
    if (flush == 0)
    {
        ++g_feed;
        if (stream->avail_in != 1 || stream->total_in >= g_size) ++g_error;
        ++stream->total_in;
        ++stream->total_out;
        ++stream->next_in;
        ++stream->next_out;
        return 0;
    }
    if (flush != 4) ++g_error;
    ++g_finish;
    if (g_finish == 1) return 0;
    ++stream->total_out;
    return 1;
}
extern "C" int __cdecl z_deflateEnd(z_stream_s* stream)
{
    ++g_end;
    if (g_finish != 2 || stream->total_out != (unsigned long)(g_feed + 1)) ++g_error;
    return 0;
}
void* __cdecl CCompressorZlib::Alloc(void*, unsigned int, unsigned int) { return 0; }
void __cdecl CCompressorZlib::Free(void*, void*) {}

int main()
{
    static union { double align; unsigned char bytes[sizeof(CCompressorZlib)]; } storage;
    CCompressorZlib* compressor = reinterpret_cast<CCompressorZlib*>(&storage);
    unsigned char input[16], output[16];
    for (unsigned i = 0; i < sizeof(input); ++i) input[i] = (unsigned char)i;
    const unsigned long sizes[] = {0, 1, 3, 8};
    const unsigned long capacities[] = {0, 1, 2, 9};
    int cases = 0;
    for (int s = 0; s < 4; ++s) for (int c = 0; c < 4; ++c)
    {
        g_init = g_feed = g_finish = g_end = g_error = 0;
        g_input = input; g_output = output;
        g_size = sizes[s]; g_capacity = capacities[c];
        unsigned long result = compressor->InternalCompress(input, g_size, output, g_capacity);
        if (!g_size)
        {
            if (result || g_init || g_feed || g_finish || g_end || g_error) return 1;
        }
        else
        {
            unsigned long expected = g_size < g_capacity ? g_size : g_capacity;
            if (g_init != 1 || g_feed != (int)expected || g_finish != 2 || g_end != 1 ||
                result != expected + 1 || g_error) return 2;
        }
        ++cases;
    }
    printf("ZLIB_INTERNAL_COMPRESS_PASS cases=%d\n", cases);
    return 0;
}
