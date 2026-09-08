struct CCreatureActionBase {};
struct CCreatureAction_ScorpionChargeInto : CCreatureActionBase {
    unsigned char State[0x120];
    CCreatureAction_ScorpionChargeInto(const CCreatureAction_ScorpionChargeInto& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ScorpionChargeInto::Clone() const { return new CCreatureAction_ScorpionChargeInto(*this); }