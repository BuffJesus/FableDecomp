struct CCreatureActionBase {};
struct CCreatureAction_BriarRoseConcentrateInto : CCreatureActionBase {
    unsigned char State[0x120];
    CCreatureAction_BriarRoseConcentrateInto(const CCreatureAction_BriarRoseConcentrateInto& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_BriarRoseConcentrateInto::Clone() const { return new CCreatureAction_BriarRoseConcentrateInto(*this); }