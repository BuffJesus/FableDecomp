struct CCreatureActionBase {};
struct CCreatureAction_ShowItemToThing : CCreatureActionBase {
    unsigned char State[0xb8];
    CCreatureAction_ShowItemToThing(const CCreatureAction_ShowItemToThing& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ShowItemToThing::Clone() const { return new CCreatureAction_ShowItemToThing(*this); }