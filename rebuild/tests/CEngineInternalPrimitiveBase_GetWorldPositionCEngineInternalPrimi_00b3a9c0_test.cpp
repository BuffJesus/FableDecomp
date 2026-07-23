#include <cstdio>

struct C3DVector { float x, y, z; };

bool __fastcall CEngineInternalPrimitiveBase_GetWorldPosition(void* self, int edx, C3DVector* out)
{
    (void)self;
    (void)edx;
    (void)out;
    return false;
}

int main()
{
    C3DVector v = { 1.0f, 2.0f, 3.0f };
    char obj[8];
    bool r = CEngineInternalPrimitiveBase_GetWorldPosition(obj, 0, &v);
    if (r != false) { std::printf("FAIL: expected false\n"); return 1; }
    std::printf("CEngineInternalPrimitiveBase_00b3a9c0_TEST PASS\n");
    return 0;
}