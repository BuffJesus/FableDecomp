#include <cstdio>
typedef unsigned long ulong;

ulong __fastcall CCombatAbilityBlockLightWeaponAttackDef_GetSizeofClass(void* self)
{
    (void)self;
    return 0x58;
}

int main()
{
    int dummy = 0;
    ulong r = CCombatAbilityBlockLightWeaponAttackDef_GetSizeofClass(&dummy);
    if (r == 0x58) {
        std::printf("CCombatAbilityBlockLightWeaponAttackDef_00453e9a_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}