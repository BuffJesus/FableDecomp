struct CCreatureActionBase {};
struct CCreatureAction_ShopkeeperGetOut : CCreatureActionBase {
    unsigned char State[0xd4];
    CCreatureAction_ShopkeeperGetOut(const CCreatureAction_ShopkeeperGetOut& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ShopkeeperGetOut::Clone() const { return new CCreatureAction_ShopkeeperGetOut(*this); }