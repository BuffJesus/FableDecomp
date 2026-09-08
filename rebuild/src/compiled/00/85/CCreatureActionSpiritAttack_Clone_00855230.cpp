struct CCreatureActionBase {};
struct CCreatureAction_SpiritAttack : CCreatureActionBase {
    unsigned char State[0xb8];
    CCreatureAction_SpiritAttack(const CCreatureAction_SpiritAttack& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_SpiritAttack::Clone() const { return new CCreatureAction_SpiritAttack(*this); }