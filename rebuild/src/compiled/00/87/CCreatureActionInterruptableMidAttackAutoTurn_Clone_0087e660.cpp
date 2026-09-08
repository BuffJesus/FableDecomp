struct CCreatureActionBase {};
struct CCreatureAction_InterruptableMidAttackAutoTurn : CCreatureActionBase {
    unsigned char State[0x12c];
    CCreatureAction_InterruptableMidAttackAutoTurn(const CCreatureAction_InterruptableMidAttackAutoTurn& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_InterruptableMidAttackAutoTurn::Clone() const { return new CCreatureAction_InterruptableMidAttackAutoTurn(*this); }