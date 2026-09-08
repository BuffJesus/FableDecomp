struct CCreatureActionBase {};
struct CCreatureAction_CastBattleCharge : CCreatureActionBase {
    unsigned char State[0x130];
    CCreatureAction_CastBattleCharge(const CCreatureAction_CastBattleCharge& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_CastBattleCharge::Clone() const { return new CCreatureAction_CastBattleCharge(*this); }