struct CCreatureActionBase {};
struct CCreatureAction_JumpOutFromCover : CCreatureActionBase {
    unsigned char State[0x120];
    CCreatureAction_JumpOutFromCover(const CCreatureAction_JumpOutFromCover& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_JumpOutFromCover::Clone() const { return new CCreatureAction_JumpOutFromCover(*this); }