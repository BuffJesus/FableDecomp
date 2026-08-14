// Byte-exact reconstruction of CCombatAbilityBlockLightWeaponAttackDef::Copy @ 0x00461b98
// __fastcall void Copy(This*, CDefClassBase const*)

struct CDefClassBase { };

// Embedded sub-object at +0x28 that has its own Copy(CDefClassBase const*)
struct SubDef {
    char pad[4];
    void Copy(const CDefClassBase* src);
};

// Base class portion
struct BaseDef {
    char pad[4];
    void Copy(const CDefClassBase* src);
};

struct CCombatAbilityBlockLightWeaponAttackDef : public BaseDef {
    // offset 0x28: embedded sub-def
    char gap[0x28 - sizeof(BaseDef)];
    SubDef sub;
    void Copy(const CDefClassBase* src);
};

void CCombatAbilityBlockLightWeaponAttackDef::Copy(const CDefClassBase* src)
{
    BaseDef::Copy(src);
    this->sub.Copy((const CDefClassBase*)((const char*)src + 0x28));
}