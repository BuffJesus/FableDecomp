struct CCreatureActionBase {};
struct CCreatureAction_ShowTrophy : CCreatureActionBase {
    unsigned char State[0xc0];
    CCreatureAction_ShowTrophy(const CCreatureAction_ShowTrophy& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ShowTrophy::Clone() const { return new CCreatureAction_ShowTrophy(*this); }