struct CCreatureActionBase {};
struct CCreatureAction_BanditKingStuck : CCreatureActionBase {
    unsigned char State[0x128];
    CCreatureAction_BanditKingStuck(const CCreatureAction_BanditKingStuck& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_BanditKingStuck::Clone() const { return new CCreatureAction_BanditKingStuck(*this); }