struct CCreatureActionBase {};
struct CCreatureAction_AimMissileWeapon : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_AimMissileWeapon(const CCreatureAction_AimMissileWeapon& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_AimMissileWeapon::Clone() const { return new CCreatureAction_AimMissileWeapon(*this); }