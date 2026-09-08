struct CCreatureActionBase {};
struct CCreatureAction_AddRealObjectToInventory : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_AddRealObjectToInventory(const CCreatureAction_AddRealObjectToInventory& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_AddRealObjectToInventory::Clone() const { return new CCreatureAction_AddRealObjectToInventory(*this); }