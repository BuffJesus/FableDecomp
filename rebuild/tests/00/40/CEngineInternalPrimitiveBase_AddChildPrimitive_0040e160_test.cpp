#include <cstdio>

struct CEngineInternalPrimitiveBase { unsigned char pad[4]; };

unsigned long __fastcall CEngineInternalPrimitiveBase_AddChildPrimitive(CEngineInternalPrimitiveBase* self)
{
    (void)self;
    return 0;
}

int main()
{
    CEngineInternalPrimitiveBase obj;
    unsigned long r = CEngineInternalPrimitiveBase_AddChildPrimitive(&obj);
    if (r != 0) { std::printf("FAIL: expected 0 got %lu\n", r); return 1; }
    std::printf("CEngineInternalPrimitiveBase_0040e160_TEST PASS\n");
    return 0;
}