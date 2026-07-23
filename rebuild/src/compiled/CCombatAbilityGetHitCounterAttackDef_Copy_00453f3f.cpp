struct CBaseDef {
    unsigned char _b[0x28];
    void Copy(CBaseDef* other);
};
struct CMemberDef {
    unsigned char _m[0x20];
    void Copy(CMemberDef* other);
};
struct CCombatAbilityGetHitCounterAttackDef : public CBaseDef {
    CMemberDef m_member; // at +0x28
    unsigned char _tail[0x48 - 0x28 - 0x20];
    void Copy(CCombatAbilityGetHitCounterAttackDef* other);
};

void CCombatAbilityGetHitCounterAttackDef::Copy(CCombatAbilityGetHitCounterAttackDef* other)
{
    CBaseDef::Copy((CBaseDef*)other);
    this->m_member.Copy(&other->m_member);
}