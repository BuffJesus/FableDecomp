#include <cstdio>

struct CEngineInternalPrimitiveOcclusionBase;
struct CEngineInternalPrimitiveBase;

CEngineInternalPrimitiveOcclusionBase* __fastcall CEngineInternalPrimitiveBase_GetAsOcclusionPrimitive(CEngineInternalPrimitiveBase* self)
{
    (void)self;
    return 0;
}

int main()
{
    CEngineInternalPrimitiveBase* obj = (CEngineInternalPrimitiveBase*)0x12345678;
    CEngineInternalPrimitiveOcclusionBase* r = CEngineInternalPrimitiveBase_GetAsOcclusionPrimitive(obj);
    if (r != 0) { std::printf("FAIL expected null\n"); return 1; }
    std::printf("CEngineInternalPrimitiveBase_00b3aa80_TEST PASS\n");
    return 0;
}