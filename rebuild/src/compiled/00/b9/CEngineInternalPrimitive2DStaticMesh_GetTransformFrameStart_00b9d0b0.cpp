#include "engine/CEngineInternalPrimitive2DStaticMesh.h"  // retyped onto the PDB layout; byte parity re-verified

struct CMatrix3x4 {
    float m[12];
};

bool __fastcall GetTransformFrameStart(
    CEngineInternalPrimitive2DStaticMesh* self, int /*edx*/, CMatrix3x4& out)
{
    out = *(CMatrix3x4*)&self->transform;
    return true;
}
