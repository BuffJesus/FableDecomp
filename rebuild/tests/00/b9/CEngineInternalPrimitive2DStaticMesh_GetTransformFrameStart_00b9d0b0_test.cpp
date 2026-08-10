#include <stdio.h>

struct CMatrix3x4 { float m[12]; };

struct CEngineInternalPrimitive2DStaticMesh {
    char pad[0x70];
    CMatrix3x4 frame;
};

bool __fastcall GetTransformFrameStart(CEngineInternalPrimitive2DStaticMesh* self, int, CMatrix3x4& out);

int main()
{
    CEngineInternalPrimitive2DStaticMesh obj;
    for (int i = 0; i < 12; ++i) obj.frame.m[i] = (float)(i * 3 + 1);
    CMatrix3x4 dst;
    for (int i = 0; i < 12; ++i) dst.m[i] = -1.0f;
    bool r = GetTransformFrameStart(&obj, 0, dst);
    bool ok = r;
    for (int i = 0; i < 12; ++i) if (dst.m[i] != (float)(i * 3 + 1)) ok = false;
    if (ok) printf("00b9d0b0_TEST PASS\n");
    else printf("FAIL\n");
    return 0;
}