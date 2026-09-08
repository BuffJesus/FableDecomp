struct CCreatureActionBase {};
struct CCreatureAction_DowseFire : CCreatureActionBase {
    unsigned char State[0xbc];
    CCreatureAction_DowseFire(const CCreatureAction_DowseFire& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_DowseFire::Clone() const { return new CCreatureAction_DowseFire(*this); }