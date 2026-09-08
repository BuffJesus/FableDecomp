struct CCreatureActionBase {};
struct CCreatureAction_ScorpionChargeStrike : CCreatureActionBase {
    unsigned char State[0x124];
    CCreatureAction_ScorpionChargeStrike(const CCreatureAction_ScorpionChargeStrike& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ScorpionChargeStrike::Clone() const { return new CCreatureAction_ScorpionChargeStrike(*this); }