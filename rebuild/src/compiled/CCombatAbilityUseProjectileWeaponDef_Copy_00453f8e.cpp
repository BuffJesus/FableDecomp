struct CCombatAbilityUseProjectileWeaponDef;

extern void __fastcall base_copy(CCombatAbilityUseProjectileWeaponDef* self, void* edx, const CCombatAbilityUseProjectileWeaponDef* other);

void __fastcall CCombatAbilityUseProjectileWeaponDef_Copy(CCombatAbilityUseProjectileWeaponDef* self, void* edx, const CCombatAbilityUseProjectileWeaponDef* other)
{
    base_copy(self, edx, other);
}