struct CCreatureActionBase {};
struct CCreatureAction_PickCrop : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_PickCrop(const CCreatureAction_PickCrop& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PickCrop::Clone() const { return new CCreatureAction_PickCrop(*this); }