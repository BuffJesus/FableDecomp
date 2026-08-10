struct CMatrix3x4 {
    float m[12];
};

struct CEngineInternalPrimitive2DStaticMesh {
    char pad[0x70];
    CMatrix3x4 frameEnd; // at +0x70
};

bool __fastcall GetTransformFrameEnd(CEngineInternalPrimitive2DStaticMesh* self, int /*edx*/, CMatrix3x4& out)
{
    out = self->frameEnd;
    return true;
}