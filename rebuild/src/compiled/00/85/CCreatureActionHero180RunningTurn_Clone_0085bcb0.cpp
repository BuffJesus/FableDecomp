struct CCreatureActionBase {};
struct CCreatureActionHero180RunningTurn : CCreatureActionBase {
    unsigned char State[0x120];
    CCreatureActionHero180RunningTurn(const CCreatureActionHero180RunningTurn& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureActionHero180RunningTurn::Clone() const { return new CCreatureActionHero180RunningTurn(*this); }