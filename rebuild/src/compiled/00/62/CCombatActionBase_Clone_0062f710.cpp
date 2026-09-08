struct CCreatureActionBase {};
struct CCombatActionBase : CCreatureActionBase {
    unsigned char State[0x114];
    CCombatActionBase(const CCombatActionBase& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCombatActionBase::Clone() const {
    return new CCombatActionBase(*this);
}