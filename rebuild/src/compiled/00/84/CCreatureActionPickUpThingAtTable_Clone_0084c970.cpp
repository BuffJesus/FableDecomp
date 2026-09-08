struct CCreatureActionBase {};
struct CCreatureAction_PickUpThingAtTable : CCreatureActionBase {
    unsigned char State[0xb4];
    CCreatureAction_PickUpThingAtTable(const CCreatureAction_PickUpThingAtTable& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PickUpThingAtTable::Clone() const { return new CCreatureAction_PickUpThingAtTable(*this); }