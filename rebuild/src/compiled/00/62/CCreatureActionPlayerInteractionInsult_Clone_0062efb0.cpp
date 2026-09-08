struct CCreatureActionBase {};
struct CCreatureAction_PlayerInteractionInsult : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_PlayerInteractionInsult(const CCreatureAction_PlayerInteractionInsult& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PlayerInteractionInsult::Clone() const {
    return new CCreatureAction_PlayerInteractionInsult(*this);
}