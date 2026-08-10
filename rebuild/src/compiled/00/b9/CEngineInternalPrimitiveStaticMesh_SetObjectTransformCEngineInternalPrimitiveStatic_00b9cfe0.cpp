struct CMatrix3x4 { float m[12]; };

struct CEngineInternalPrimitiveStaticMesh {
    char pad[0x70];
    CMatrix3x4 transform;
};

bool __fastcall SetObjectTransform(CEngineInternalPrimitiveStaticMesh* self, int /*edx*/, const CMatrix3x4& mat)
{
    self->transform = mat;
    return true;
}