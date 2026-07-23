struct CBaseDef {
    void Copy(const CBaseDef* other);
};

struct CMemberDef {
    void Copy(const CMemberDef* other);
};

struct CCombatAbilityFlourishCounterAttackDef {
    unsigned char _base[0x28];
    CMemberDef m_member; // at +0x28
    unsigned char _tail[0x18 - sizeof(CMemberDef)];

    void Copy(const CCombatAbilityFlourishCounterAttackDef* other);
};

void CCombatAbilityFlourishCounterAttackDef::Copy(
        const CCombatAbilityFlourishCounterAttackDef* other)
{
    ((CBaseDef*)this)->Copy((const CBaseDef*)other);
    m_member.Copy(&other->m_member);
}