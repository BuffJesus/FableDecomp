struct CCreatureActionBase {};
struct CCreatureAction_BattleChargeMiss : CCreatureActionBase {
    unsigned char State[0x124];
    CCreatureAction_BattleChargeMiss(const CCreatureAction_BattleChargeMiss& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_BattleChargeMiss::Clone() const { return new CCreatureAction_BattleChargeMiss(*this); }