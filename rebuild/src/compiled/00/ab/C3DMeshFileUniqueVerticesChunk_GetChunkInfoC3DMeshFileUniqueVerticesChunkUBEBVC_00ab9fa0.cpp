#include "engine/C3DMeshFileUniqueVerticesChunk.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString {
    CCharString(const char* lit, int len);
    int dummy;
};

struct C3DMeshFileUniqueVerticesChunk_Methods : C3DMeshFileUniqueVerticesChunk {
    CCharString GetChunkInfo() const;
};

CCharString C3DMeshFileUniqueVerticesChunk_Methods::GetChunkInfo() const
{
    return CCharString((const char*)0x122d70e, -1);
}