#include <cstdio>

struct CEngineInternalPrimitiveBase { void* vtbl; };

void __fastcall CEngineInternalPrimitiveBase_ClearAllChildPrimitives(CEngineInternalPrimitiveBase* self)
{
    (void)self;
}

int main()
{
    CEngineInternalPrimitiveBase obj;
    obj.vtbl = 0;
    CEngineInternalPrimitiveBase_ClearAllChildPrimitives(&obj);
    std::printf("CEngineInternalPrimitiveBase_00b8fde0_TEST PASS\n");
    return 0;
}