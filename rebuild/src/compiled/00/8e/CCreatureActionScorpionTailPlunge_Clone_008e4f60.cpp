struct CCreatureActionBase {};
struct CCreatureAction_ScorpionTailPlunge : CCreatureActionBase {
    unsigned char State[0x120];
    CCreatureAction_ScorpionTailPlunge(const CCreatureAction_ScorpionTailPlunge& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ScorpionTailPlunge::Clone() const { return new CCreatureAction_ScorpionTailPlunge(*this); }