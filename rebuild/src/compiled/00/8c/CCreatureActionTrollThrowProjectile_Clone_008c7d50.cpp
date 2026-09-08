struct CCreatureActionBase {};
struct CCreatureAction_TrollThrowProjectile : CCreatureActionBase {
    unsigned char State[0x130];
    CCreatureAction_TrollThrowProjectile(const CCreatureAction_TrollThrowProjectile& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_TrollThrowProjectile::Clone() const { return new CCreatureAction_TrollThrowProjectile(*this); }