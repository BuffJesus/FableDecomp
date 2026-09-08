struct CCreatureActionBase {};
struct CCreatureAction_CastPhysicalShieldSpell : CCreatureActionBase {
    unsigned char State[0x124];
    CCreatureAction_CastPhysicalShieldSpell(const CCreatureAction_CastPhysicalShieldSpell& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_CastPhysicalShieldSpell::Clone() const { return new CCreatureAction_CastPhysicalShieldSpell(*this); }