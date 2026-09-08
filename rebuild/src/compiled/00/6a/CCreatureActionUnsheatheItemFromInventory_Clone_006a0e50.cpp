struct CCreatureActionBase {};
struct CCreatureAction_UnsheatheItemFromInventory : CCreatureActionBase {
    unsigned char State[0xb8];
    CCreatureAction_UnsheatheItemFromInventory(const CCreatureAction_UnsheatheItemFromInventory& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_UnsheatheItemFromInventory::Clone() const {
    return new CCreatureAction_UnsheatheItemFromInventory(*this);
}