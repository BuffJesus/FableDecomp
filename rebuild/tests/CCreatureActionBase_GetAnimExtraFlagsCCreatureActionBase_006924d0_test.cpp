#include <cstdio>

struct CCreatureActionBase { void* vtbl; };

unsigned long __fastcall CCreatureActionBase_GetAnimExtraFlags(const CCreatureActionBase* self)
{
    (void)self;
    return 0;
}

int main()
{
    CCreatureActionBase obj;
    obj.vtbl = 0;
    unsigned long r = CCreatureActionBase_GetAnimExtraFlags(&obj);
    if (r != 0) {
        std::printf("CCreatureActionBase_006924d0_TEST FAIL r=%lu\n", r);
        return 1;
    }
    std::printf("CCreatureActionBase_006924d0_TEST PASS\n");
    return 0;
}