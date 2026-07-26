#include <cstdio>

struct CCombatAbilityUseProjectileWeaponDef { char pad[4]; };

unsigned long __fastcall CCombatAbilityUseProjectileWeaponDef_GetSizeofClass(const CCombatAbilityUseProjectileWeaponDef* self)
{
    (void)self;
    return 0x48;
}

int main()
{
    CCombatAbilityUseProjectileWeaponDef obj;
    unsigned long r = CCombatAbilityUseProjectileWeaponDef_GetSizeofClass(&obj);
    if (r != 0x48) { std::printf("FAIL got %lu\n", r); return 1; }
    std::printf("CCombatAbilityUseProjectileWeaponDef_0044c5b9_TEST PASS\n");
    return 0;
}