// Behaviour fixture for CCompressorZlib::SetStreamInput 0x00A3AB90, linked against the provider object (it does not
// define the member): the stream gets the new input, the output buffer is cleared, nothing else is touched.
#include "engine/CCompressorZlib.h"
#include <stdio.h>
#include <string.h>

int main()
{
    static unsigned char storage[sizeof(CCompressorZlib)];
    memset(storage, 0xAB, sizeof(storage));
    CCompressorZlib* zlib = reinterpret_cast<CCompressorZlib*>(storage);
    const char input[16] = "fable";

    zlib->SetStreamInput(input, 12345);

    const z_stream_s& s = zlib->Stream;
    bool ok = s.next_in == (const unsigned char*)input && s.avail_in == 12345 && s.next_out == 0 && s.avail_out == 0;
    // total_in (+0x08) and everything outside the four fields keep their bytes.
    const size_t base = offsetof(CCompressorZlib, Stream);
    for (size_t i = 0; i < sizeof(storage); ++i)
    {
        const bool written = (i >= base && i < base + 8) || (i >= base + 12 && i < base + 20);
        if (!written && storage[i] != 0xAB) ok = false;
    }
    if (ok)
    {
        printf("SETSTREAMINPUT_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}
