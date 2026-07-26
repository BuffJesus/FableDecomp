#include <cstdio>

typedef unsigned long ulong;

ulong __fastcall CCombatAbilityFlourishCounterAttackDef_GetSizeofClass(void* self)
{
    (void)self;
    return 0x48;
}

int main()
{
    int dummy = 0;
    ulong r = CCombatAbilityFlourishCounterAttackDef_GetSizeofClass(&dummy);
    if (r == 0x48) {
        std::printf("CCombatAbilityFlourishCounterAttackDef_0044c561_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL: got %lu\n", r);
    return 1;
}