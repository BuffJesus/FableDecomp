struct CCreatureActionBase {};
struct CCreatureAction_TeleportOut : CCreatureActionBase {
    unsigned char State[0x124];
    CCreatureAction_TeleportOut(const CCreatureAction_TeleportOut& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_TeleportOut::Clone() const { return new CCreatureAction_TeleportOut(*this); }