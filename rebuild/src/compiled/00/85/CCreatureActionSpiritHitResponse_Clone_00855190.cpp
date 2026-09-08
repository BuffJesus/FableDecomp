struct CCreatureActionBase {};
struct CCreatureAction_SpiritHitResponse : CCreatureActionBase {
    unsigned char State[0xb4];
    CCreatureAction_SpiritHitResponse(const CCreatureAction_SpiritHitResponse& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_SpiritHitResponse::Clone() const { return new CCreatureAction_SpiritHitResponse(*this); }