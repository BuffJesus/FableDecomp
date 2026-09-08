struct CCreatureActionBase {};
struct CCreatureAction_BattleChargeImpact : CCreatureActionBase {
    unsigned char State[0x124];
    CCreatureAction_BattleChargeImpact(const CCreatureAction_BattleChargeImpact& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_BattleChargeImpact::Clone() const { return new CCreatureAction_BattleChargeImpact(*this); }