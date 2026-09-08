struct CCreatureActionBase {};
struct CCreatureAction_PlayIntoLoopOutOfAnimation : CCreatureActionBase {
    unsigned char State[0xc4];
    CCreatureAction_PlayIntoLoopOutOfAnimation(const CCreatureAction_PlayIntoLoopOutOfAnimation& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PlayIntoLoopOutOfAnimation::Clone() const { return new CCreatureAction_PlayIntoLoopOutOfAnimation(*this); }