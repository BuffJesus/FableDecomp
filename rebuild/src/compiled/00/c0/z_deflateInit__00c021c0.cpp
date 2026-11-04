#include "fable_zlib.h"

extern "C" int __cdecl z_deflateInit_(z_stream_s* stream, int level,
                                       const char* version, int stream_size)
{
    return z_deflateInit2_(stream, level, 8, 15, 8, 0, version, stream_size);
}
