#include <stdio.h>

struct CMatrix3x4 {
    float m[12];
};

struct CEngineInternalPrimitive2DStaticMesh {
    char pad[0x70];
    CMatrix3x4 transform;
};

bool __fastcall CEngineInternalPrimitive2DStaticMesh_GetObjectTransform(
    CEngineInternalPrimitive2DStaticMesh* self, int edx, CMatrix3x4& out);

int main() {
    CEngineInternalPrimitive2DStaticMesh obj;
    for (int i = 0; i < 12; ++i) obj.transform.m[i] = (float)(i * 3 + 1);
    CMatrix3x4 dst;
    for (int i = 0; i < 12; ++i) dst.m[i] = -1.0f;
    bool r = CEngineInternalPrimitive2DStaticMesh_GetObjectTransform(&obj, 0, dst);
    bool ok = r;
    for (int i = 0; i < 12; ++i) if (dst.m[i] != (float)(i * 3 + 1)) ok = false;
    if (ok) printf("00b9d070_TEST PASS\n");
    else printf("FAIL\n");
    return 0;
}