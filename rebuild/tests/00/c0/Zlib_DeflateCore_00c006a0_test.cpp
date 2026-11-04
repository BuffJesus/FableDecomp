#define Z_PREFIX
extern "C" {
#include "third_party/zlib121/deflate.h"
}
#include <stdio.h>
#include <string.h>

extern "C" const char* const z_errmsg[10] = {"", "", "", "", "", "", "", "", "", ""};
extern "C" const unsigned char _dist_code[512] = {0};
extern "C" const unsigned char _length_code[256] = {0};
extern "C" uLong z_adler32(uLong, const Bytef*, uInt) { return 1; }
extern "C" uLong z_crc32(uLong, const Bytef*, uInt) { return 0; }
extern "C" uLong z_compressBound(uLong n) { return n + 16; }
extern "C" int z_deflateEnd(z_streamp) { return 0; }
extern "C" void _tr_init(deflate_state*) {}
extern "C" void _tr_align(deflate_state*) {}
extern "C" void _tr_stored_block(deflate_state*, charf*, ulg, int) {}
extern "C" void _tr_flush_block(deflate_state*, charf*, ulg, int) {}

int main()
{
    if (deflateReset(0) != Z_STREAM_ERROR || deflate(0, Z_NO_FLUSH) != Z_STREAM_ERROR)
        return 1;
    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    if (deflateReset(&stream) != Z_STREAM_ERROR || deflate(&stream, Z_NO_FLUSH) != Z_STREAM_ERROR)
        return 2;
    puts("ZLIB_DEFLATE_CORE_LINK_PASS cases=4");
    return 0;
}
