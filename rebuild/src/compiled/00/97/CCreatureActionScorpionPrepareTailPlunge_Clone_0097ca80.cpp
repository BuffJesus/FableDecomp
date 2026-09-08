struct CCreatureActionBase {};
struct CCreatureAction_ScorpionPrepareTailPlunge : CCreatureActionBase {
    unsigned char State[0x120];
    CCreatureAction_ScorpionPrepareTailPlunge(const CCreatureAction_ScorpionPrepareTailPlunge& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ScorpionPrepareTailPlunge::Clone() const { return new CCreatureAction_ScorpionPrepareTailPlunge(*this); }