struct CCreatureActionBase {};
struct CCreatureAction_PickUpGenericBox : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_PickUpGenericBox(const CCreatureAction_PickUpGenericBox& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PickUpGenericBox::Clone() const { return new CCreatureAction_PickUpGenericBox(*this); }