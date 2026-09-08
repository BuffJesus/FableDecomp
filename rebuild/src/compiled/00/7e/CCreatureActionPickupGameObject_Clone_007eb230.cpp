struct CCreatureActionBase {};
struct CCreatureAction_PickupGameObject : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_PickupGameObject(const CCreatureAction_PickupGameObject& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PickupGameObject::Clone() const { return new CCreatureAction_PickupGameObject(*this); }