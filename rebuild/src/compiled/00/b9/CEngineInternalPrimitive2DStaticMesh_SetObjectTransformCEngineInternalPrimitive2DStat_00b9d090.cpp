#include "engine/CEngineInternalPrimitive2DStaticMesh.h"  // retyped onto the PDB layout; byte parity re-verified

class CMatrix3x4 {
public:
    float m[12];
};

bool __fastcall CEngineInternalPrimitive2DStaticMesh_SetObjectTransform(
    CEngineInternalPrimitive2DStaticMesh* self, int /*edx*/, const CMatrix3x4& matrix)
{
    *(CMatrix3x4*)&self->transform = matrix;
    return true;
}
