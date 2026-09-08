struct CCreatureActionBase {};
struct CCombatAction_GenericStrikeResponseKnockdown : CCreatureActionBase {
    unsigned char State[0x13c];
    CCombatAction_GenericStrikeResponseKnockdown(const CCombatAction_GenericStrikeResponseKnockdown& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCombatAction_GenericStrikeResponseKnockdown::Clone() const { return new CCombatAction_GenericStrikeResponseKnockdown(*this); }