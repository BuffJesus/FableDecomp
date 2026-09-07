#include "engine/CCompressedMemoryDataInputStream.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"

// CCompressedMemoryDataInputStream::NeedsBufferedRead @ 0x00994590
// __fastcall: returns (requested < this->m_threshold at +0x1c), signed-less.
// One stack arg, ret 4.
struct CCompressedMemoryDataInputStream_Methods : CCompressedMemoryDataInputStream {
    bool NeedsBufferedRead(int requested);
};

bool CCompressedMemoryDataInputStream_Methods::NeedsBufferedRead(int requested)
{
    int threshold = *(const int*)((const char*)this + 0x1c);
    return requested < threshold;
}