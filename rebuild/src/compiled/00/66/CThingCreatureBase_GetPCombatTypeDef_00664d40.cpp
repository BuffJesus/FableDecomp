#define FABLE_THING_CONST_GETTC_PROVIDER
#include "fable_creature_combat.h"
#include "fable_definition_manager.h"
CDefPointer<const CCombatTypeDef> CThingCreatureBase::GetPCombatTypeDef() const
{
    const bool hasCombat=HasTC(TCI_COMBAT);
    if(hasCombat)
    {
        const CTCCombat& combat=static_cast<const CTCCombat&>(GetTC(TCI_COMBAT));
        CDefPointer<const CCombatTypeDef> componentDef;
        componentDef=combat.GetPCombatTypeDef();
        if(componentDef.Get())return componentDef;
    }
    CDefPointer<const CCombatTypeDef> def;
    long index=GetCombatType();
    if(index!=0)PeekDefinitionManager().GetDef<CCombatTypeDef>(index,&def);
    else {
        CCharString nullCombatType("COMBAT_TYPE_NULL");
        PeekDefinitionManager().GetDef<CCombatTypeDef>(nullCombatType,&def);
    }
    return def;
}
