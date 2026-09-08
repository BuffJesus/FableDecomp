struct CCreatureActionBase {};
struct CCreatureAction_PlayerInteractionBribe : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_PlayerInteractionBribe(const CCreatureAction_PlayerInteractionBribe& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PlayerInteractionBribe::Clone() const {
    return new CCreatureAction_PlayerInteractionBribe(*this);
}