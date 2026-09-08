struct CCreatureActionBase {};
struct CCreatureAction_PlayerInteractionFlirt : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_PlayerInteractionFlirt(const CCreatureAction_PlayerInteractionFlirt& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PlayerInteractionFlirt::Clone() const {
    return new CCreatureAction_PlayerInteractionFlirt(*this);
}