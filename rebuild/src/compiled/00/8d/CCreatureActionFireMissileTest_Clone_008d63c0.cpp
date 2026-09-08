struct CCreatureActionBase {};
struct CCreatureAction_FireMissileTest : CCreatureActionBase {
    unsigned char State[0xb4];
    CCreatureAction_FireMissileTest(const CCreatureAction_FireMissileTest& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_FireMissileTest::Clone() const { return new CCreatureAction_FireMissileTest(*this); }