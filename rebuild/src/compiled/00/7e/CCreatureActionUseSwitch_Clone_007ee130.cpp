struct CCreatureActionBase {};
struct CCreatureAction_UseSwitch : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_UseSwitch(const CCreatureAction_UseSwitch& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_UseSwitch::Clone() const { return new CCreatureAction_UseSwitch(*this); }