#include "engine/CEngineInternalPrimitive2DStaticMesh.h"  // retyped onto the PDB layout; byte parity re-verified

class CMatrix3x4 {
public:
    float m[12];
};

bool __fastcall GetTransformFrameStart(
    CEngineInternalPrimitive2DStaticMesh* self, int /*edx*/, CMatrix3x4& out)
{
    out = *(CMatrix3x4*)&self->transform;
    return true;
}
