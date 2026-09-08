struct CCreatureActionBase {};
struct CCreatureAction_HeroLoadRangedWeapon : CCreatureActionBase {
    unsigned char State[0xb8];
    CCreatureAction_HeroLoadRangedWeapon(const CCreatureAction_HeroLoadRangedWeapon& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_HeroLoadRangedWeapon::Clone() const { return new CCreatureAction_HeroLoadRangedWeapon(*this); }