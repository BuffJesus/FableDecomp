#include <cstdio>

struct CMatrix3x4 { float m[12]; };

struct CEngineInternalPrimitiveStaticMesh {
    char pad[0x70];
    CMatrix3x4 transform;
};

bool __fastcall CEngineInternalPrimitiveStaticMesh_GetObjectTransform(
    CEngineInternalPrimitiveStaticMesh* self, int, CMatrix3x4& out);

int main() {
    CEngineInternalPrimitiveStaticMesh obj;
    for (int i = 0; i < 12; ++i) obj.transform.m[i] = (float)(i + 1);
    CMatrix3x4 out;
    for (int i = 0; i < 12; ++i) out.m[i] = -1.0f;
    bool r = CEngineInternalPrimitiveStaticMesh_GetObjectTransform(&obj, 0, out);
    bool ok = r;
    for (int i = 0; i < 12; ++i) if (out.m[i] != (float)(i + 1)) ok = false;
    if (ok) printf("00b9cfc0_TEST PASS\n");
    else printf("FAIL\n");
    return 0;
}