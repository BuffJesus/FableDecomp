struct CCreatureActionBase {};
struct CCreatureAction_ScreamerDie : CCreatureActionBase {
    unsigned char State[0x11c];
    CCreatureAction_ScreamerDie(const CCreatureAction_ScreamerDie& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ScreamerDie::Clone() const { return new CCreatureAction_ScreamerDie(*this); }