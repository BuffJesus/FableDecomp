struct CCreatureActionBase {};
struct CCreatureAction_KickCreature : CCreatureActionBase {
    unsigned char State[0xb0];
    CCreatureAction_KickCreature(const CCreatureAction_KickCreature& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCreatureAction_KickCreature::Clone() const { return new CCreatureAction_KickCreature(*this); }