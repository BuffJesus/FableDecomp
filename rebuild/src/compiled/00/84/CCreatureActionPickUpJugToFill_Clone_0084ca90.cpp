struct CCreatureActionBase {};
struct CCreatureAction_PickUpJugToFill : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_PickUpJugToFill(const CCreatureAction_PickUpJugToFill& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_PickUpJugToFill::Clone() const { return new CCreatureAction_PickUpJugToFill(*this); }