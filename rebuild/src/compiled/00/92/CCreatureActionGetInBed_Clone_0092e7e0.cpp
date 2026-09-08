struct CCreatureActionBase {};
struct CCreatureAction_GetInBed : CCreatureActionBase {
    unsigned char State[0xc0];
    CCreatureAction_GetInBed(const CCreatureAction_GetInBed& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_GetInBed::Clone() const { return new CCreatureAction_GetInBed(*this); }