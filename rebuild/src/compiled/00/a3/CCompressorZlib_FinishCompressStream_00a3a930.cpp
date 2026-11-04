// CCompressorZlib::FinishCompressStream() @ 0x00A3A930 (Ego_r ?FinishCompressStream@CCompressorZlib@@QAEXXZ).
// Ends an open deflate stream (zlib z_deflateEnd 0x00C00AE0) and clears the initialised flag.
#include "engine/CCompressorZlib.h"

void CCompressorZlib::FinishCompressStream()
{
    if (ZlibInitialised)
    {
        z_deflateEnd(&Stream);
        ZlibInitialised = false;
    }
}
