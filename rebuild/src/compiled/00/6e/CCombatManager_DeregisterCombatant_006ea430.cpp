// Byte-exact reconstruction of CCombatManager::DeregisterCombatant @ 0x006ea430
struct CThingCreatureBase
{
    // 0x662410 : bool __fastcall (membership / in-combat predicate)
    char IsCombatant();
};

struct CCombatManager
{
    // 0x6ea2c0 : void __fastcall CCombatManager::RemoveCombatant(CThingCreatureBase*)
    void RemoveCombatant(CThingCreatureBase* creature);
};

// self arrives in ecx; model this-in-ecx as the leading __fastcall pointer param.
void __fastcall CCombatManager_DeregisterCombatant(CCombatManager* self, void* edx, CThingCreatureBase* creature)
{
    if (creature->IsCombatant())
        self->RemoveCombatant(creature);
}