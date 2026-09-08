#include "engine/C3DMeshFileTrianglesChunk.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString { char* Data; CCharString(const char* text, int length); };
struct C3DMeshFileTrianglesChunk_Methods : C3DMeshFileTrianglesChunk {
    CCharString GetChunkDescription() const;
};
CCharString C3DMeshFileTrianglesChunk_Methods::GetChunkDescription() const
{
    return CCharString("Triangles", -1);
}
