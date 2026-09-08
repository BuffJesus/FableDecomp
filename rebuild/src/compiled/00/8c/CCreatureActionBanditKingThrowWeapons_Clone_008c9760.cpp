struct CCreatureActionBase {};
struct CCreatureAction_BanditKingThrowWeapons : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_BanditKingThrowWeapons(const CCreatureAction_BanditKingThrowWeapons& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_BanditKingThrowWeapons::Clone() const { return new CCreatureAction_BanditKingThrowWeapons(*this); }