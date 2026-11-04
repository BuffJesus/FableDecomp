#include "fable_zlib.h"
#include <stdio.h>

static z_stream_s* g_stream;
static int g_level, g_method, g_window, g_memory, g_strategy, g_size, g_calls;
static const char* g_version;
extern "C" int __cdecl z_deflateInit2_(z_stream_s* stream, int level, int method,
                                       int window, int memory, int strategy,
                                       const char* version, int size)
{
    ++g_calls;
    g_stream = stream; g_level = level; g_method = method; g_window = window;
    g_memory = memory; g_strategy = strategy; g_version = version; g_size = size;
    return level + size;
}
int main()
{
    z_stream_s stream;
    const char version[] = "1.2.1";
    for (int level = -1; level <= 9; ++level)
    {
        int result = z_deflateInit_(&stream, level, version, sizeof(stream));
        if (result != level + sizeof(stream) || g_stream != &stream || g_level != level ||
            g_method != 8 || g_window != 15 || g_memory != 8 || g_strategy != 0 ||
            g_version != version || g_size != sizeof(stream) || g_calls != level + 2) return 1;
    }
    puts("Z_DEFLATE_INIT_PASS cases=11");
    return 0;
}
