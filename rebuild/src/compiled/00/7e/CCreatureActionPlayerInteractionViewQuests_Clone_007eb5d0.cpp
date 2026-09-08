struct CCreatureActionBase {};
struct CCreatureAction_PlayerInteractionViewQuests : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_PlayerInteractionViewQuests(const CCreatureAction_PlayerInteractionViewQuests& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PlayerInteractionViewQuests::Clone() const { return new CCreatureAction_PlayerInteractionViewQuests(*this); }