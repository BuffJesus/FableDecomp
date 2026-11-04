#include "third_party/zlib121/retail_headers.h"
#include "third_party/zlib121/deflate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" const char* const z_errmsg[10] = {
    "need dictionary", "stream end", "", "stream error", "data error",
    "buffer error", "memory error", "version error", "", ""
};
static int g_allocs, g_frees, g_fail_at, g_resets, g_ends, g_bad;
static voidpf __cdecl RecordAlloc(voidpf opaque, unsigned items, unsigned size)
{
    if (opaque != &g_allocs) ++g_bad;
    ++g_allocs;
    if (g_allocs == g_fail_at) return 0;
    return malloc(items * size);
}
static void __cdecl RecordFree(voidpf opaque, voidpf ptr)
{
    if (opaque != &g_allocs || !ptr) ++g_bad;
    ++g_frees;
    free(ptr);
}
extern "C" voidpf zcalloc(voidpf, unsigned items, unsigned size) { return malloc(items * size); }
extern "C" void zcfree(voidpf, voidpf ptr) { free(ptr); }
extern "C" int z_deflateReset(z_streamp stream)
{
    ++g_resets;
    deflate_state* state = (deflate_state*)stream->state;
    if (!state || state->strm != stream || !state->window || !state->prev ||
        !state->head || !state->pending_buf || state->w_size == 0 ||
        state->pending_buf_size == 0) ++g_bad;
    return 7;
}
extern "C" int z_deflateEnd(z_streamp stream)
{
    ++g_ends;
    deflate_state* state = (deflate_state*)stream->state;
    if (!state) { ++g_bad; return -2; }
    if (state->pending_buf) stream->zfree(stream->opaque, state->pending_buf);
    if (state->head) stream->zfree(stream->opaque, state->head);
    if (state->prev) stream->zfree(stream->opaque, state->prev);
    if (state->window) stream->zfree(stream->opaque, state->window);
    stream->zfree(stream->opaque, state);
    stream->state = 0;
    return 0;
}

int main()
{
    int cases = 0;
    z_stream stream;
    for (int variant = 0; variant < 7; ++variant)
    {
        memset(&stream, 0, sizeof(stream));
        stream.zalloc = RecordAlloc; stream.zfree = RecordFree; stream.opaque = &g_allocs;
        g_allocs = g_frees = g_resets = g_ends = g_bad = 0;
        g_fail_at = 0;
        int level = variant == 1 ? -1 : 1;
        int window = variant == 2 ? -15 : variant == 3 ? 31 : variant == 4 ? 8 : 15;
        int mem = variant == 5 ? 1 : 8;
        int strategy = variant == 6 ? 2 : 0;
        int result = z_deflateInit2_(&stream, level, 8, window, mem, strategy, "1.2.1", sizeof(stream));
        if (result != 7 || g_resets != 1 || g_allocs != 5 || g_bad) return 1;
        deflate_state* state = (deflate_state*)stream.state;
        if (state->wrap != (window < 0 ? 0 : window > 15 ? 2 : 1) ||
            state->w_bits != (window < 0 ? 15 : window > 15 ? 15 : window == 8 ? 9 : 15) ||
            state->level != (level == -1 ? 6 : level) || state->strategy != strategy ||
            state->method != 8) return 2;
        z_deflateEnd(&stream);
        if (g_ends != 1 || g_frees != 5 || stream.state || g_bad) return 3;
        ++cases;
    }
    for (int bad = 0; bad < 10; ++bad)
    {
        memset(&stream, 0, sizeof(stream));
        stream.zalloc = RecordAlloc; stream.zfree = RecordFree; stream.opaque = &g_allocs;
        g_allocs = g_frees = g_resets = g_ends = g_bad = 0; g_fail_at = 0;
        int level = bad == 0 ? -2 : bad == 1 ? 10 : 1;
        int method = bad == 2 ? 7 : 8;
        int window = bad == 3 ? 7 : bad == 4 ? 32 : 15;
        int mem = bad == 5 ? 0 : bad == 6 ? 10 : 8;
        int strategy = bad == 7 ? -1 : bad == 8 ? 4 : 0;
        int result = z_deflateInit2_(&stream, level, method, window, mem, strategy,
                                     bad == 9 ? "2.0" : "1.2.1", sizeof(stream));
        if (result != (bad == 9 ? -6 : -2) || g_allocs || g_resets || g_ends || g_bad) return 4;
        ++cases;
    }
    for (int fail = 1; fail <= 5; ++fail)
    {
        memset(&stream, 0, sizeof(stream));
        stream.zalloc = RecordAlloc; stream.zfree = RecordFree; stream.opaque = &g_allocs;
        g_allocs = g_frees = g_resets = g_ends = g_bad = 0; g_fail_at = fail;
        int result = z_deflateInit2_(&stream, 1, 8, 15, 8, 0, "1.2.1", sizeof(stream));
        if (result != -4 || g_allocs != (fail == 1 ? 1 : 5) || g_resets || g_bad) return 5;
        if (fail == 1)
        {
            if (g_ends || g_frees || stream.state) return 6;
        }
        else if (g_ends != 1 || g_frees != 4 || stream.state ||
                 stream.msg != z_errmsg[6]) return 7;
        ++cases;
    }
    memset(&stream, 0, sizeof(stream));
    if (z_deflateInit2_(0, 1, 8, 15, 8, 0, "1.2.1", sizeof(stream)) != -2) return 8;
    ++cases;
    if (z_deflateInit2_(&stream, 1, 8, 15, 8, 0, "1.2.1", sizeof(stream) - 4) != -6) return 9;
    ++cases;
    puts("Z_DEFLATE_INIT2_PASS cases=24");
    return cases == 24 ? 0 : 10;
}
