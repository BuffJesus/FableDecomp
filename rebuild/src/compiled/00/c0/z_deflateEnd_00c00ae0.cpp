#include "fable_zlib.h"
#include <stddef.h>

// Only the fields read by the retail deflate-end path are named here.
struct internal_state
{
    unsigned char unknown_00[4];
    int status;
    void* allocation_08;
    unsigned char unknown_0c[0x24];
    void* allocation_30;
    unsigned char unknown_34[4];
    void* allocation_38;
    void* allocation_3c;
};
FABLE_STATIC_ASSERT(offsetof(internal_state, allocation_3c) == 0x3c);

extern "C" int __cdecl z_deflateEnd(z_stream_s* stream)
{
    if (stream != 0 && stream->state != 0)
    {
        int status = stream->state->status;
        if (status != 0x2a && status != 0x71 && status != 0x29a) return -2;
        if (stream->state->allocation_08) stream->zfree(stream->opaque, stream->state->allocation_08);
        if (stream->state->allocation_3c) stream->zfree(stream->opaque, stream->state->allocation_3c);
        if (stream->state->allocation_38) stream->zfree(stream->opaque, stream->state->allocation_38);
        if (stream->state->allocation_30) stream->zfree(stream->opaque, stream->state->allocation_30);
        stream->zfree(stream->opaque, stream->state);
        stream->state = 0;
        return status == 0x71 ? -3 : 0;
    }
    return -2;
}
