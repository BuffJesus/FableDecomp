#include <stdio.h>

struct CMatrix3x4 { float m[12]; };

struct CEngineInternalPrimitive2DStaticMesh {
    char pad[0x70];
    CMatrix3x4 transform;
};

bool __fastcall CEngineInternalPrimitive2DStaticMesh_SetObjectTransform(
    CEngineInternalPrimitive2DStaticMesh* self, int edx, const CMatrix3x4& m);

int main() {
    CEngineInternalPrimitive2DStaticMesh obj;
    for (int i = 0; i < (int)sizeof(obj); ++i) ((char*)&obj)[i] = 0;
    CMatrix3x4 src;
    for (int i = 0; i < 12; ++i) src.m[i] = (float)(i + 1) * 1.5f;

    bool r = CEngineInternalPrimitive2DStaticMesh_SetObjectTransform(&obj, 0, src);

    bool ok = r;
    for (int i = 0; i < 12; ++i)
        if (obj.transform.m[i] != src.m[i]) ok = false;

    if (ok) printf("00b9d090_TEST PASS\n");
    else printf("FAIL\n");
    return 0;
}