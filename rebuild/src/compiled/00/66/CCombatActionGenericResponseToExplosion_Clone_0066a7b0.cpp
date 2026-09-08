struct CCreatureActionBase {};
struct CCombatAction_GenericResponseToExplosion : CCreatureActionBase {
    unsigned char State[0x138];
    CCombatAction_GenericResponseToExplosion(const CCombatAction_GenericResponseToExplosion& source);
    CCreatureActionBase* Clone() const;
};
CCreatureActionBase* CCombatAction_GenericResponseToExplosion::Clone() const {
    return new CCombatAction_GenericResponseToExplosion(*this);
}