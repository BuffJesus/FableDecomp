struct CCreatureActionBase {};
struct CCreatureAction_LearnExpression : CCreatureActionBase {
    unsigned char State[0xb8];
    CCreatureAction_LearnExpression(const CCreatureAction_LearnExpression& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_LearnExpression::Clone() const { return new CCreatureAction_LearnExpression(*this); }