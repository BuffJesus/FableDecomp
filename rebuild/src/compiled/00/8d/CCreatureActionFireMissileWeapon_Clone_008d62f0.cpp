struct CCreatureActionBase {};
struct CCreatureAction_FireMissileWeapon : CCreatureActionBase {
    unsigned char State[0xc8];
    CCreatureAction_FireMissileWeapon(const CCreatureAction_FireMissileWeapon& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_FireMissileWeapon::Clone() const { return new CCreatureAction_FireMissileWeapon(*this); }