struct CCreatureActionBase {};
struct CCreatureAction_DeclarePlayerACriminal : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_DeclarePlayerACriminal(const CCreatureAction_DeclarePlayerACriminal& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_DeclarePlayerACriminal::Clone() const { return new CCreatureAction_DeclarePlayerACriminal(*this); }