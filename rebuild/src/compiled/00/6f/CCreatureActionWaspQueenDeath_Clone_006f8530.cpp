struct CCreatureActionBase {};
struct CCreatureAction_WaspQueenDeath : CCreatureActionBase {
    unsigned char State[0x11c];
    CCreatureAction_WaspQueenDeath(const CCreatureAction_WaspQueenDeath& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_WaspQueenDeath::Clone() const { return new CCreatureAction_WaspQueenDeath(*this); }