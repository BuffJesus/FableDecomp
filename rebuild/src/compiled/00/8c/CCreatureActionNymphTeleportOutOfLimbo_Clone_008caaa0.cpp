struct CCreatureActionBase {};
struct CCreatureAction_NymphTeleportOutOfLimbo : CCreatureActionBase {
    unsigned char State[0x120];
    CCreatureAction_NymphTeleportOutOfLimbo(const CCreatureAction_NymphTeleportOutOfLimbo& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_NymphTeleportOutOfLimbo::Clone() const { return new CCreatureAction_NymphTeleportOutOfLimbo(*this); }