struct CCreatureActionBase {};
struct CCreatureAction_JackScream : CCreatureActionBase {
    unsigned char State[0x11c];
    CCreatureAction_JackScream(const CCreatureAction_JackScream& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_JackScream::Clone() const { return new CCreatureAction_JackScream(*this); }