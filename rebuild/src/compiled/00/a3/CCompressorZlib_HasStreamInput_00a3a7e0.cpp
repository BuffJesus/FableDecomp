#include "engine/CCompressorZlib.h"

bool CCompressorZlib::HasStreamInput() const
{
    return Stream.avail_in != 0;
}
