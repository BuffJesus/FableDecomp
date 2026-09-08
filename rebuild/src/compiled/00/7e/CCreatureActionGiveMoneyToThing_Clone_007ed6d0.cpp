struct CCreatureActionBase {};
struct CCreatureAction_GiveMoneyToThing : CCreatureActionBase {
    unsigned char State[0xb4];
    CCreatureAction_GiveMoneyToThing(const CCreatureAction_GiveMoneyToThing& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_GiveMoneyToThing::Clone() const { return new CCreatureAction_GiveMoneyToThing(*this); }