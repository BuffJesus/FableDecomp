struct CThingCreatureBase {
    bool IsCombatant();
};

struct CCombatManager {
    void RemoveCombatant(CThingCreatureBase* c);
    void DeregisterCombatant(CThingCreatureBase* c);
};

void CCombatManager::DeregisterCombatant(CThingCreatureBase* c)
{
    if (c->IsCombatant())
        this->RemoveCombatant(c);
}