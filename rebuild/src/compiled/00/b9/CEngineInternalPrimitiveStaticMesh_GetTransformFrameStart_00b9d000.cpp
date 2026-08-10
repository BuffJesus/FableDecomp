struct CMatrix3x4 {
    float m[12];
};

struct CEngineInternalPrimitiveStaticMesh {
    char pad[0x70];
    CMatrix3x4 frame; // at +0x70
};

bool __fastcall GetTransformFrameStart(
    CEngineInternalPrimitiveStaticMesh* self, int /*edx*/, CMatrix3x4& out)
{
    out = self->frame;
    return true;
}