struct CCreatureActionBase {};
struct CCreatureAction_UseWallMount : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_UseWallMount(const CCreatureAction_UseWallMount& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_UseWallMount::Clone() const { return new CCreatureAction_UseWallMount(*this); }