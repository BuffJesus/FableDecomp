#include <stdio.h>

struct CMatrix3x4 {
    float m[12];
};

struct CEngineInternalPrimitive2DStaticMesh {
    char pad[0x70];
    CMatrix3x4 frameEnd;
};

bool __fastcall GetTransformFrameEnd(CEngineInternalPrimitive2DStaticMesh* self, int /*edx*/, CMatrix3x4& out)
{
    out = self->frameEnd;
    return true;
}

int main()
{
    CEngineInternalPrimitive2DStaticMesh obj;
    for (int i = 0; i < 12; ++i) obj.frameEnd.m[i] = (float)(i + 1) * 1.5f;
    CMatrix3x4 dst;
    for (int i = 0; i < 12; ++i) dst.m[i] = -1.0f;
    bool r = GetTransformFrameEnd(&obj, 0, dst);
    bool ok = r;
    for (int i = 0; i < 12; ++i) {
        if (dst.m[i] != (float)(i + 1) * 1.5f) ok = false;
    }
    if (ok) printf("00b9d0d0_TEST PASS\n");
    else printf("FAIL\n");
    return 0;
}