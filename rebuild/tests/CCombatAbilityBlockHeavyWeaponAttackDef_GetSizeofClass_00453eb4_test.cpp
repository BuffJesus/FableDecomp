#include <cstdio>

struct CCombatAbilityBlockHeavyWeaponAttackDef { char pad[0x58]; };

unsigned long __fastcall CCombatAbilityBlockHeavyWeaponAttackDef_GetSizeofClass(const CCombatAbilityBlockHeavyWeaponAttackDef* self)
{
    (void)self;
    return 0x58;
}

int main()
{
    CCombatAbilityBlockHeavyWeaponAttackDef obj;
    unsigned long r = CCombatAbilityBlockHeavyWeaponAttackDef_GetSizeofClass(&obj);
    if (r == 0x58) {
        std::printf("CCombatAbilityBlockHeavyWeaponAttackDef_00453eb4_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}