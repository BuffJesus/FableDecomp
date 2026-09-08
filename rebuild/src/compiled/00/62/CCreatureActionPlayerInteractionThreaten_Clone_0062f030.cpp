struct CCreatureActionBase {};
struct CCreatureAction_PlayerInteractionThreaten : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_PlayerInteractionThreaten(const CCreatureAction_PlayerInteractionThreaten& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PlayerInteractionThreaten::Clone() const {
    return new CCreatureAction_PlayerInteractionThreaten(*this);
}