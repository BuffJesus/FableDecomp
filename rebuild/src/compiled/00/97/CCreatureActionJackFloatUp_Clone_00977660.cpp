struct CCreatureActionBase {};
struct CCreatureAction_JackFloatUp : CCreatureActionBase {
    unsigned char State[0x124];
    CCreatureAction_JackFloatUp(const CCreatureAction_JackFloatUp& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_JackFloatUp::Clone() const { return new CCreatureAction_JackFloatUp(*this); }