#include "engine/CFileDataInputStream.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"

// CFileDataInputStream::NeedsBufferedRead @ 0x009943b0
// __fastcall: returns (requested < this->m_threshold at +0x20), signed-less.
// One stack arg, ret 4.
struct CFileDataInputStream_Methods : CFileDataInputStream {
    bool NeedsBufferedRead(int requested);
};

bool CFileDataInputStream_Methods::NeedsBufferedRead(int requested)
{
    int threshold = *(const int*)((const char*)this + 0x20);
    return requested < threshold;
}