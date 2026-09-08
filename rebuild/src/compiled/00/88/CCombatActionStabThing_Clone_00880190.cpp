struct CCreatureActionBase {};
struct CCombatAction_StabThing : CCreatureActionBase {
    unsigned char State[0x120];
    CCombatAction_StabThing(const CCombatAction_StabThing& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCombatAction_StabThing::Clone() const { return new CCombatAction_StabThing(*this); }