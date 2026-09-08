#include "engine/C3DMeshFileSubMeshChunk.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString { char* Data; int Length; CCharString(const char* text, int length); };
struct C3DMeshFileSubMeshChunk_Methods : C3DMeshFileSubMeshChunk {
    CCharString GetChunkDescription() const;
};
CCharString C3DMeshFileSubMeshChunk_Methods::GetChunkDescription() const
{
    return CCharString("SubMesh", -1);
}
