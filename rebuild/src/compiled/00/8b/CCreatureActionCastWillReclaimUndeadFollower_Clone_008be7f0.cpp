struct CCreatureActionBase {};
struct CCreatureAction_CastWillReclaimUndeadFollower : CCreatureActionBase {
    unsigned char State[0x11c];
    CCreatureAction_CastWillReclaimUndeadFollower(const CCreatureAction_CastWillReclaimUndeadFollower& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_CastWillReclaimUndeadFollower::Clone() const { return new CCreatureAction_CastWillReclaimUndeadFollower(*this); }