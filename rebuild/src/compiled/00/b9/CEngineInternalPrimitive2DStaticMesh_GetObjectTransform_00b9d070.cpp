struct CMatrix3x4 {
    float m[12];
};

struct CEngineInternalPrimitive2DStaticMesh {
    char pad[0x70];
    CMatrix3x4 transform;
};

bool __fastcall CEngineInternalPrimitive2DStaticMesh_GetObjectTransform(
    CEngineInternalPrimitive2DStaticMesh* self, int /*edx*/, CMatrix3x4& out)
{
    out = self->transform;
    return true;
}