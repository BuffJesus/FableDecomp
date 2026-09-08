struct CCreatureActionBase {};
struct CCreatureAction_ReloadMissileWeapon : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_ReloadMissileWeapon(const CCreatureAction_ReloadMissileWeapon& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_ReloadMissileWeapon::Clone() const { return new CCreatureAction_ReloadMissileWeapon(*this); }