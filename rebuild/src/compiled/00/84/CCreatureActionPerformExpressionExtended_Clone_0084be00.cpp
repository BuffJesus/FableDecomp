struct CCreatureActionBase {};
struct CCreatureAction_PerformExpressionExtended : CCreatureActionBase {
    unsigned char State[0xc8];
    CCreatureAction_PerformExpressionExtended(const CCreatureAction_PerformExpressionExtended& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PerformExpressionExtended::Clone() const { return new CCreatureAction_PerformExpressionExtended(*this); }