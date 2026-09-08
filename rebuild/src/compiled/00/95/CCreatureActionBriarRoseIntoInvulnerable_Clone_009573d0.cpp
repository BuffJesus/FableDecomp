struct CCreatureActionBase {};
struct CCreatureAction_BriarRoseIntoInvulnerable : CCreatureActionBase {
    unsigned char State[0x11c];
    CCreatureAction_BriarRoseIntoInvulnerable(const CCreatureAction_BriarRoseIntoInvulnerable& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_BriarRoseIntoInvulnerable::Clone() const { return new CCreatureAction_BriarRoseIntoInvulnerable(*this); }