struct CCreatureActionBase {};
struct CCreatureAction_OpenChest : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_OpenChest(const CCreatureAction_OpenChest& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_OpenChest::Clone() const { return new CCreatureAction_OpenChest(*this); }