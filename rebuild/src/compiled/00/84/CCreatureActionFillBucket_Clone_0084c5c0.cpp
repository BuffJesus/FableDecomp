struct CCreatureActionBase {};
struct CCreatureAction_FillBucket : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_FillBucket(const CCreatureAction_FillBucket& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_FillBucket::Clone() const { return new CCreatureAction_FillBucket(*this); }