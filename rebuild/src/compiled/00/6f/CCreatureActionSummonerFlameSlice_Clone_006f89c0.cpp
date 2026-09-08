struct CCreatureActionBase {};
struct CCreatureAction_SummonerFlameSlice : CCreatureActionBase {
    unsigned char State[0x11c];
    CCreatureAction_SummonerFlameSlice(const CCreatureAction_SummonerFlameSlice& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_SummonerFlameSlice::Clone() const { return new CCreatureAction_SummonerFlameSlice(*this); }