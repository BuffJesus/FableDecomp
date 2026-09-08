struct CCreatureActionBase {};
struct CCreatureAction_NymphGetHit : CCreatureActionBase {
    unsigned char State[0x154];
    CCreatureAction_NymphGetHit(const CCreatureAction_NymphGetHit& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_NymphGetHit::Clone() const { return new CCreatureAction_NymphGetHit(*this); }