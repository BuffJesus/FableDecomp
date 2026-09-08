struct CCreatureActionBase {};
struct CCreatureAction_SheatheItemToInventory : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_SheatheItemToInventory(const CCreatureAction_SheatheItemToInventory& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_SheatheItemToInventory::Clone() const {
    return new CCreatureAction_SheatheItemToInventory(*this);
}