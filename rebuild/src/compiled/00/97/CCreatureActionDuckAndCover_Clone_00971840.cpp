struct CCreatureActionBase {};
struct CCreatureAction_DuckAndCover : CCreatureActionBase {
    unsigned char State[0x120];
    CCreatureAction_DuckAndCover(const CCreatureAction_DuckAndCover& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_DuckAndCover::Clone() const { return new CCreatureAction_DuckAndCover(*this); }