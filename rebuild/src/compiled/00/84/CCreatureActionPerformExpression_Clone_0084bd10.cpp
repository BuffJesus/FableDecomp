struct CCreatureActionBase {};
struct CCreatureAction_PerformExpression : CCreatureActionBase {
    unsigned char State[0xc8];
    CCreatureAction_PerformExpression(const CCreatureAction_PerformExpression& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PerformExpression::Clone() const { return new CCreatureAction_PerformExpression(*this); }