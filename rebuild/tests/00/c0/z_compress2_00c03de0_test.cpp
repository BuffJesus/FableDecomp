#define Z_PREFIX
extern "C" {
#include "third_party/zlib121/zlib.h"
}
#include <stdio.h>

static int g_init_result, g_deflate_result, g_end_result;
static int g_init_calls, g_deflate_calls, g_end_calls, g_level;
static z_streamp g_stream;
static Bytef *g_input, *g_output;
static uInt g_input_length;

extern "C" int z_deflateInit_(z_streamp stream, int level, const char*, int)
{
    ++g_init_calls;
    g_stream = stream;
    g_level = level;
    return g_init_result;
}
extern "C" int z_deflate(z_streamp stream, int flush)
{
    ++g_deflate_calls;
    if (stream != g_stream || flush != Z_FINISH) return Z_STREAM_ERROR;
    g_input = stream->next_in;
    g_output = stream->next_out;
    g_input_length = stream->avail_in;
    stream->total_out = 17;
    return g_deflate_result;
}
extern "C" int z_deflateEnd(z_streamp stream)
{
    ++g_end_calls;
    return stream == g_stream ? g_end_result : Z_STREAM_ERROR;
}

int main()
{
    Bytef source[4] = {1, 2, 3, 4};
    Bytef output[64] = {0};
    const int levels[] = {-1, 0, 9};
    for (unsigned i = 0; i < 3; ++i)
    {
        uLongf length = sizeof(output);
        g_init_result = Z_OK; g_deflate_result = Z_STREAM_END; g_end_result = Z_OK;
        if (compress2(output, &length, source, sizeof(source), levels[i]) != Z_OK ||
            length != 17 || g_level != levels[i] || g_input != source ||
            g_output != output || g_input_length != sizeof(source)) return 1;
    }
    uLongf length = sizeof(output);
    g_init_result = Z_STREAM_ERROR;
    if (compress2(output, &length, source, sizeof(source), 13) != Z_STREAM_ERROR ||
        g_deflate_calls != 3 || g_end_calls != 3) return 2;
    g_init_result = Z_OK; g_deflate_result = Z_OK;
    if (compress2(output, &length, source, sizeof(source), 6) != Z_BUF_ERROR) return 3;
    g_deflate_result = Z_STREAM_ERROR;
    if (compress2(output, &length, source, sizeof(source), 6) != Z_STREAM_ERROR) return 4;
    g_deflate_result = Z_STREAM_END; g_end_result = Z_DATA_ERROR;
    if (compress2(output, &length, source, sizeof(source), 6) != Z_DATA_ERROR ||
        g_init_calls != 7 || g_deflate_calls != 6 || g_end_calls != 6) return 5;
    puts("Z_COMPRESS2_PASS cases=7");
    return 0;
}
