struct CCreatureActionBase {};
struct CCreatureAction_BriarRoseInvulnerableTeleport : CCreatureActionBase {
    unsigned char State[0x11c];
    CCreatureAction_BriarRoseInvulnerableTeleport(const CCreatureAction_BriarRoseInvulnerableTeleport& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_BriarRoseInvulnerableTeleport::Clone() const { return new CCreatureAction_BriarRoseInvulnerableTeleport(*this); }