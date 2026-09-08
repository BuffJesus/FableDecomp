#include "engine/C3DMeshFileXVertexDataChunk.h"  // retyped onto the PDB layout; byte parity re-verified

struct CCharString {
    char* Data;
    CCharString(const char* text, int length);
};

__declspec(noinline) CCharString::CCharString(const char* text, int length)
{
    Data = (char*)text;
    if (length) Data = (char*)text + 1;
}

struct C3DMeshFileXVertexDataChunk_Methods : C3DMeshFileXVertexDataChunk {
    CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileXVertexDataChunk_Methods::GetChunkDescription() const
{
    return CCharString((const char*)0x129e368, -1);
}
