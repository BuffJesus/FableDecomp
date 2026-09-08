struct CCreatureActionBase {};
struct CCreatureAction_UseLamp : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_UseLamp(const CCreatureAction_UseLamp& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_UseLamp::Clone() const { return new CCreatureAction_UseLamp(*this); }