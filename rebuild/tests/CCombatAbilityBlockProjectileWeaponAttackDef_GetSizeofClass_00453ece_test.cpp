#include <cstdio>

typedef unsigned long ulong;

struct CCombatAbilityBlockProjectileWeaponAttackDef {
    int dummy;
};

ulong __fastcall CCombatAbilityBlockProjectileWeaponAttackDef_GetSizeofClass(const CCombatAbilityBlockProjectileWeaponAttackDef* self)
{
    return 0x58;
}

int main()
{
    CCombatAbilityBlockProjectileWeaponAttackDef obj;
    ulong r = CCombatAbilityBlockProjectileWeaponAttackDef_GetSizeofClass(&obj);
    if (r == 0x58) {
        std::printf("CCombatAbilityBlockProjectileWeaponAttackDef_00453ece_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}