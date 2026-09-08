struct CCreatureActionBase {};
struct CCreatureAction_PlayerInteractionGreet : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_PlayerInteractionGreet(const CCreatureAction_PlayerInteractionGreet& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PlayerInteractionGreet::Clone() const {
    return new CCreatureAction_PlayerInteractionGreet(*this);
}