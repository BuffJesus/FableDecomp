/* zlib 1.2.1 compress.c; see third_party/zlib121/zlib.h for license. */
#define Z_PREFIX
extern "C" {
#include "third_party/zlib121/zlib.h"
}

uLong ZEXPORT compressBound(uLong sourceLen)
{
    return sourceLen + (sourceLen >> 12) + (sourceLen >> 14) + 11;
}
