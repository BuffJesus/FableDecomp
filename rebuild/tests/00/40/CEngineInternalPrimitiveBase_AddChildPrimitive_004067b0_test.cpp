#include <cstdio>
typedef unsigned long ulong;
struct CEngineInternalPrimitiveBase {};
ulong __fastcall CEngineInternalPrimitiveBase_AddChildPrimitive(CEngineInternalPrimitiveBase* self, void* a)
{
    (void)self; (void)a;
    return 0x32;
}
int main()
{
    CEngineInternalPrimitiveBase obj;
    ulong r = CEngineInternalPrimitiveBase_AddChildPrimitive(&obj, 0);
    if (r != 0x32) { std::printf("FAIL got %lu\n", r); return 1; }
    std::printf("CEngineInternalPrimitiveBase_004067b0_TEST PASS\n");
    return 0;
}