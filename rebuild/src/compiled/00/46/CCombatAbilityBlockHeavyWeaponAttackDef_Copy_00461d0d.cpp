
struct CMember { void Copy(const CMember* src); };
struct CBaseDef { void Copy(const CBaseDef* src); };
struct CCombatAbilityBlockHeavyWeaponAttackDef : public CBaseDef {
    unsigned char _pad[0x48];
};

void __fastcall CCombatAbilityBlockHeavyWeaponAttackDef_Copy(CCombatAbilityBlockHeavyWeaponAttackDef* self, void* edx, const CCombatAbilityBlockHeavyWeaponAttackDef* src)
{
    ((CBaseDef*)self)->Copy((const CBaseDef*)src);
    CMember* m = (CMember*)((unsigned char*)self + 0x28);
    m->Copy((const CMember*)((const unsigned char*)src + 0x28));
}