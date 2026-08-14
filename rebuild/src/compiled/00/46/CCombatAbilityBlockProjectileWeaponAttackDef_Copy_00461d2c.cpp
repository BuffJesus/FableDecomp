// Byte-exact reconstruction of CCombatAbilityBlockProjectileWeaponAttackDef::Copy @ 0x00461d2c
// Native __fastcall throughout (real member functions), so no edx is ever set.
//   push esi; push edi; mov edi,[esp+0xc]; push edi; mov esi,ecx;
//   call BaseDef::Copy (this=self, src pushed)
//   add edi,0x28; push edi; lea ecx,[esi+0x28]; call Member28::Copy
//   pop edi; pop esi; ret 4

struct CDefClassBase;

// Member subobject type living at self+0x28.  Copy @ 0x461bb7.
struct Member28 {
    void Copy(const Member28* src);   // extern (masked rel32)
};

// Base def of the combat-ability def.  Copy @ 0x431f10 (takes CDefClassBase const*).
struct BaseDef {
    void Copy(const CDefClassBase* src);   // extern (masked rel32)
};

struct CCombatAbilityBlockProjectileWeaponAttackDef {
    BaseDef  base;                                 // +0x00 .. +0x28
    char     _pad[0x28 - sizeof(BaseDef)];
    Member28 sub28;                                // +0x28
    void Copy(const CDefClassBase* src);
};

void CCombatAbilityBlockProjectileWeaponAttackDef::Copy(const CDefClassBase* src)
{
    base.Copy(src);
    sub28.Copy((const Member28*)((const char*)src + 0x28));
}