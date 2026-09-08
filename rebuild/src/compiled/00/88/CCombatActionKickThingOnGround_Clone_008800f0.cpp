struct CCreatureActionBase {};
struct CCombatAction_KickThingOnGround : CCreatureActionBase {
    unsigned char State[0x130];
    CCombatAction_KickThingOnGround(const CCombatAction_KickThingOnGround& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCombatAction_KickThingOnGround::Clone() const { return new CCombatAction_KickThingOnGround(*this); }