struct CCreatureActionBase {};
struct CCreatureAction_GetOutOfBed : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_GetOutOfBed(const CCreatureAction_GetOutOfBed& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_GetOutOfBed::Clone() const { return new CCreatureAction_GetOutOfBed(*this); }