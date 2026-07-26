#include <cstdio>

struct CCombatAbilityUseProjectileWeaponDef { int calls; };

static int g_called = 0;

void __fastcall base_copy(CCombatAbilityUseProjectileWeaponDef* self, void* edx, const CCombatAbilityUseProjectileWeaponDef* other)
{
    (void)edx; (void)other;
    g_called++;
    if (self) self->calls++;
}

void __fastcall CCombatAbilityUseProjectileWeaponDef_Copy(CCombatAbilityUseProjectileWeaponDef* self, void* edx, const CCombatAbilityUseProjectileWeaponDef* other)
{
    base_copy(self, edx, other);
}

int main()
{
    CCombatAbilityUseProjectileWeaponDef a; a.calls = 0;
    CCombatAbilityUseProjectileWeaponDef b; b.calls = 0;
    CCombatAbilityUseProjectileWeaponDef_Copy(&a, 0, &b);
    if (g_called == 1 && a.calls == 1) {
        std::printf("CCombatAbilityUseProjectileWeaponDef_00453f8e_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}