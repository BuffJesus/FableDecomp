#include <stdio.h>

struct CMatrix3x4 { float m[12]; };

struct CEngineInternalPrimitiveStaticMesh {
    char pad[0x70];
    CMatrix3x4 frameEnd;
    bool GetTransformFrameEnd(CMatrix3x4 &out);
};

bool CEngineInternalPrimitiveStaticMesh::GetTransformFrameEnd(CMatrix3x4 &out)
{
    out = this->frameEnd;
    return true;
}

int main()
{
    CEngineInternalPrimitiveStaticMesh obj;
    for (int i = 0; i < 12; ++i) obj.frameEnd.m[i] = (float)(i + 1) * 1.5f;
    CMatrix3x4 out;
    for (int i = 0; i < 12; ++i) out.m[i] = -999.0f;
    bool r = obj.GetTransformFrameEnd(out);
    bool ok = r;
    for (int i = 0; i < 12; ++i)
        if (out.m[i] != (float)(i + 1) * 1.5f) ok = false;
    if (ok) printf("00b9d020_TEST PASS\n");
    else printf("00b9d020_TEST FAIL\n");
    return 0;
}