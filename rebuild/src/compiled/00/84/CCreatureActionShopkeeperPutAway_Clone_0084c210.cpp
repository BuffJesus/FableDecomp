struct CCreatureActionBase {};
struct CCreatureAction_ShopkeeperPutAway : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_ShopkeeperPutAway(const CCreatureAction_ShopkeeperPutAway& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ShopkeeperPutAway::Clone() const { return new CCreatureAction_ShopkeeperPutAway(*this); }