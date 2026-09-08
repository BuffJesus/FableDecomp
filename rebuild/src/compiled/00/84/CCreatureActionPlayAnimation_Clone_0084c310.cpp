struct CCreatureActionBase {};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char State[0xb4];
    CCreatureAction_PlayAnimation(const CCreatureAction_PlayAnimation& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PlayAnimation::Clone() const { return new CCreatureAction_PlayAnimation(*this); }