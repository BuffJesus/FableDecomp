// CCompressorZlib::SetStreamInput(const void*, unsigned long) @ 0x00A3AB90
// (Ego_r ?SetStreamInput@CCompressorZlib@@QAEXPBXK@Z). Points the stream at new input with no output buffer yet.
#include "engine/CCompressorZlib.h"

void CCompressorZlib::SetStreamInput(const void* data, unsigned long size)
{
    SetupStream(Stream, data, size, 0, 0);
}
