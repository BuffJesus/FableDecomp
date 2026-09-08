struct CCreatureActionBase {};
struct CCreatureAction_ShowLove : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_ShowLove(const CCreatureAction_ShowLove& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ShowLove::Clone() const { return new CCreatureAction_ShowLove(*this); }