#include "engine/CCompressorZlib.h"

unsigned long CCompressorZlib::InternalCompress(const void* data, unsigned long size,
                                               void* output, unsigned long capacity)
{
    if (size == 0) return 0;
    z_stream_s stream;
    stream.zalloc = Alloc;
    stream.zfree = Free;
    stream.opaque = 0;
    z_deflateInit_(&stream, 1, "1.2.1", sizeof(stream));
    stream.next_in = (unsigned char*)data;
    stream.next_out = (unsigned char*)output;
    if (stream.total_in != size)
    {
        do
        {
            if (stream.total_out >= capacity) break;
            stream.avail_out = 1;
            stream.avail_in = 1;
            z_deflate(&stream, 0);
        } while (stream.total_in != size);
    }
    int result;
    do
    {
        stream.avail_out = 1;
        result = z_deflate(&stream, 4);
    } while (result != 1);
    z_deflateEnd(&stream);
    return stream.total_out;
}
