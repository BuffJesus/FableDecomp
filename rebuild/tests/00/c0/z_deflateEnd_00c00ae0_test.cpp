#include "fable_zlib.h"
#include <stdio.h>
#include <string.h>

static void* g_freed[5];
static int g_count;
static void* g_opaque;
static void __cdecl RecordFree(void* opaque, void* pointer)
{
    if (opaque != g_opaque || g_count >= 5) { g_count = 99; return; }
    g_freed[g_count++] = pointer;
}

int main()
{
    int cases = 0;
    if (z_deflateEnd(0) != -2) return 1;
    ++cases;
    z_stream_s stream;
    memset(&stream, 0, sizeof(stream));
    if (z_deflateEnd(&stream) != -2) return 2;
    ++cases;
    unsigned char state[0x40];
    unsigned char allocations[4][4];
    const int statuses[] = {0x2a, 0x71, 0x29a, 0x777};
    const int offsets[] = {8, 0x3c, 0x38, 0x30};
    for (int s = 0; s < 4; ++s) for (int mask = 0; mask < 16; ++mask)
    {
        memset(state, 0x55, sizeof(state));
        *reinterpret_cast<int*>(state + 4) = statuses[s];
        for (int i = 0; i < 4; ++i)
            *reinterpret_cast<void**>(state + offsets[i]) = (mask & (1 << i)) ? allocations[i] : 0;
        stream.state = reinterpret_cast<internal_state*>(state);
        stream.zfree = RecordFree;
        stream.opaque = &stream;
        g_opaque = &stream;
        g_count = 0;
        int result = z_deflateEnd(&stream);
        if (s == 3)
        {
            if (result != -2 || g_count != 0 || stream.state != reinterpret_cast<internal_state*>(state)) return 3;
        }
        else
        {
            if (result != (s == 1 ? -3 : 0) || stream.state != 0) return 4;
            int expected = 0;
            for (int i = 0; i < 4; ++i)
                if (mask & (1 << i))
                    if (g_freed[expected++] != allocations[i]) return 5;
            if (g_freed[expected++] != state || g_count != expected) return 6;
        }
        ++cases;
    }
    printf("Z_DEFLATE_END_PASS cases=%d\n", cases);
    return 0;
}
