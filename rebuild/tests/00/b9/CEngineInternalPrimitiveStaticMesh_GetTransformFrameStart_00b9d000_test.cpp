#include <cstdio>
#include <cstring>

struct CMatrix3x4 {
    float m[12];
};

struct CEngineInternalPrimitiveStaticMesh {
    char pad[0x70];
    CMatrix3x4 frame;
};

bool __fastcall GetTransformFrameStart(
    CEngineInternalPrimitiveStaticMesh* self, int /*edx*/, CMatrix3x4& out);

int main() {
    CEngineInternalPrimitiveStaticMesh obj;
    memset(&obj, 0, sizeof(obj));
    for (int i = 0; i < 12; ++i) obj.frame.m[i] = (float)(i + 1) * 1.5f;

    CMatrix3x4 out;
    memset(&out, 0, sizeof(out));
    bool r = GetTransformFrameStart(&obj, 0, out);

    bool ok = r;
    for (int i = 0; i < 12; ++i)
        if (out.m[i] != (float)(i + 1) * 1.5f) ok = false;

    if (ok) printf("00b9d000_TEST PASS\n");
    else printf("FAIL\n");
    return 0;
}