#include "engine/CCompressorZlib.h"
void CCompressorZlib::Initialise()
{
    Stream.next_in = 0;
    Stream.next_out = 0;
    Stream.avail_in = 0;
    Stream.avail_out = 0;
    Stream.zalloc = Alloc;
    Stream.zfree = Free;
    ZlibInitialised = false;
    Compressing = false;
    ErrorFlag = false;
}
