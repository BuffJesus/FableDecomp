// Retail 0x00A3AB00 uses one reusable stream, independent of the object.
#include "engine/CCompressorZlib.h"
unsigned long CCompressorZlib::InternalDecompress(const void* data,unsigned long size,void* output,unsigned long capacity)
{
    if (size == 0) return 0;
    static z_stream_s stream;
    static bool initialised;
    stream.next_in = (unsigned char*)data;
    stream.avail_in = size;
    stream.next_out = (unsigned char*)output;
    stream.avail_out = capacity;
    if (!initialised)
    {
        stream.zalloc = Alloc;
        stream.zfree = Free;
        z_inflateInit_(&stream, "1.2.1", sizeof(stream));
        initialised = true;
    }
    else z_inflateReset(&stream);
    z_inflate(&stream, 4);
    return stream.total_out;
}