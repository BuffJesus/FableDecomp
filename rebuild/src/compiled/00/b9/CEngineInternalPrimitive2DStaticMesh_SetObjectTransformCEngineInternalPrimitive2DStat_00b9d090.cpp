struct CMatrix3x4 { float m[12]; };

struct CEngineInternalPrimitive2DStaticMesh {
    char pad[0x70];
    CMatrix3x4 transform;
};

bool __fastcall CEngineInternalPrimitive2DStaticMesh_SetObjectTransform(
    CEngineInternalPrimitive2DStaticMesh* self, int /*edx*/, const CMatrix3x4& m)
{
    self->transform = m;
    return true;
}