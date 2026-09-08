struct CCreatureActionBase {};
struct CCombatAction_ControlledStrafeJump : CCreatureActionBase {
    unsigned char State[0x130];
    CCombatAction_ControlledStrafeJump(const CCombatAction_ControlledStrafeJump& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCombatAction_ControlledStrafeJump::Clone() const { return new CCombatAction_ControlledStrafeJump(*this); }