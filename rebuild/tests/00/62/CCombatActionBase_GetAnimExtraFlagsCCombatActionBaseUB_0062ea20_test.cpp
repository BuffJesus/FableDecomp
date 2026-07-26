#include <cstdio>
typedef unsigned long ulong;
struct CCombatActionBase { void* vtbl; };
ulong __fastcall CCombatActionBase_GetAnimExtraFlags(const CCombatActionBase* self)
{
    (void)self;
    return 8u;
}
int main()
{
    CCombatActionBase obj;
    obj.vtbl = 0;
    if (CCombatActionBase_GetAnimExtraFlags(&obj) == 8u) {
        std::printf("CCombatActionBase_0062ea20_TEST PASS\n");
        return 0;
    }
    std::printf("CCombatActionBase_0062ea20_TEST FAIL\n");
    return 1;
}