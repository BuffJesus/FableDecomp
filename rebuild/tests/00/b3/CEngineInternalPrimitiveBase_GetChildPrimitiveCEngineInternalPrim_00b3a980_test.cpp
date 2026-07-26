#include <cstdio>

struct CEngineInternalPrimitiveBase;
struct CEnginePersistentPrimitiveBase;

CEnginePersistentPrimitiveBase* __fastcall CEngineInternalPrimitiveBase_GetChildPrimitive(CEngineInternalPrimitiveBase* self, int edx_pad, unsigned long index)
{
    (void)self;
    (void)edx_pad;
    (void)index;
    return 0;
}

int main()
{
    CEnginePersistentPrimitiveBase* r = CEngineInternalPrimitiveBase_GetChildPrimitive((CEngineInternalPrimitiveBase*)0x1234, 0, 7);
    if (r != 0) { std::printf("FAIL: expected null\n"); return 1; }
    std::printf("CEngineInternalPrimitiveBase_00b3a980_TEST PASS\n");
    return 0;
}