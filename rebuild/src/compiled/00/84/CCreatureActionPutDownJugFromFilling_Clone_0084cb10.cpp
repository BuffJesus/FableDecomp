struct CCreatureActionBase {};
struct CCreatureAction_PutDownJugFromFilling : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_PutDownJugFromFilling(const CCreatureAction_PutDownJugFromFilling& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PutDownJugFromFilling::Clone() const { return new CCreatureAction_PutDownJugFromFilling(*this); }