struct CCreatureActionBase {};
struct CCreatureAction_ProjectileCrouch : CCreatureActionBase {
    unsigned char State[0x124];
    CCreatureAction_ProjectileCrouch(const CCreatureAction_ProjectileCrouch& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ProjectileCrouch::Clone() const { return new CCreatureAction_ProjectileCrouch(*this); }