#include "engine/C3DMeshFileVerticesChunk.h"  // retyped onto the PDB layout; byte parity re-verified

struct CCharString {
    char* Data;
    CCharString(const char* text, int length);
};

__declspec(noinline) CCharString::CCharString(const char* text, int length)
{
    Data = (char*)text;
    if (length) Data = (char*)text + 1;
}

struct C3DMeshFileVerticesChunk_Methods : C3DMeshFileVerticesChunk {
    CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileVerticesChunk_Methods::GetChunkDescription() const
{
    return CCharString((const char*)0x129ed24, -1);
}
