struct CCreatureActionBase {};
struct CCreatureAction_ScorpionTailWithdraw : CCreatureActionBase {
    unsigned char State[0x120];
    CCreatureAction_ScorpionTailWithdraw(const CCreatureAction_ScorpionTailWithdraw& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ScorpionTailWithdraw::Clone() const { return new CCreatureAction_ScorpionTailWithdraw(*this); }