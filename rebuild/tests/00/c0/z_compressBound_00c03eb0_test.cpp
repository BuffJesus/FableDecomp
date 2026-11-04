#define Z_PREFIX
extern "C" {
#include "third_party/zlib121/zlib.h"
}
#include <stdio.h>

int main()
{
    const uLong inputs[] = {
        0, 1, 10, 4095, 4096, 16383, 16384,
        65535, 65536, 0x7fffffffUL, 0xffffffffUL
    };
    for (unsigned i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
    {
        uLong n = inputs[i];
        uLong expected = n + (n >> 12) + (n >> 14) + 11;
        if (compressBound(n) != expected) return 1;
    }
    puts("Z_COMPRESS_BOUND_PASS cases=11");
    return 0;
}
