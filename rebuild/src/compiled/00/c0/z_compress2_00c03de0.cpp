/* Adapted from zlib 1.2.1 compress.c for VC7.1 C++. */
#define Z_PREFIX
extern "C" {
#include "third_party/zlib121/zlib.h"
}

int ZEXPORT compress2(Bytef *dest, uLongf *destLen,
                      const Bytef *source, uLong sourceLen, int level)
{
    z_stream stream;
    int err;
    stream.next_in = (Bytef*)source;
    stream.avail_in = (uInt)sourceLen;
    stream.next_out = dest;
    stream.avail_out = (uInt)*destLen;
    if ((uLong)stream.avail_out != *destLen) return Z_BUF_ERROR;
    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;
    stream.opaque = (voidpf)0;
    err = deflateInit(&stream, level);
    if (err != Z_OK) return err;
    err = deflate(&stream, Z_FINISH);
    if (err != Z_STREAM_END) {
        deflateEnd(&stream);
        return err == Z_OK ? Z_BUF_ERROR : err;
    }
    *destLen = stream.total_out;
    err = deflateEnd(&stream);
    return err;
}
