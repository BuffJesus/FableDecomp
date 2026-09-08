struct CCreatureActionBase {};
struct CCreatureAction_Hammer : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_Hammer(const CCreatureAction_Hammer& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_Hammer::Clone() const { return new CCreatureAction_Hammer(*this); }