struct CCreatureActionBase {};
struct CCreatureAction_ScorpionCharge : CCreatureActionBase {
    unsigned char State[0x124];
    CCreatureAction_ScorpionCharge(const CCreatureAction_ScorpionCharge& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ScorpionCharge::Clone() const { return new CCreatureAction_ScorpionCharge(*this); }