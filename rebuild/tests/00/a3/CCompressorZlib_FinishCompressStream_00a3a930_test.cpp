// Behaviour fixture for CCompressorZlib::FinishCompressStream 0x00A3A930, linked against the provider object (it
// does not define the member). The compressor is laid out in raw storage at the offsets the shared header pins.
#include "engine/CCompressorZlib.h"
#include <stdio.h>
#include <string.h>

static int g_ended;
static z_stream_s* g_last;
extern "C" int __cdecl z_deflateEnd(z_stream_s* stream)
{
    ++g_ended;
    g_last = stream;
    return 0;
}

int main()
{
    static unsigned char storage[sizeof(CCompressorZlib)];
    memset(storage, 0xAB, sizeof(storage));
    CCompressorZlib* zlib = reinterpret_cast<CCompressorZlib*>(storage);

    zlib->ZlibInitialised = true;
    zlib->FinishCompressStream();
    const bool ended = g_ended == 1 && g_last == &zlib->Stream && !zlib->ZlibInitialised &&
                       storage[0x55] == 0xAB && storage[0x56] == 0xAB;

    g_ended = 0;
    zlib->FinishCompressStream();
    const bool idle = g_ended == 0 && !zlib->ZlibInitialised;

    if (ended && idle)
    {
        printf("FINISH_STREAM_OK\n");
        return 0;
    }
    printf("FAIL ended=%d idle=%d\n", ended, idle);
    return 1;
}
