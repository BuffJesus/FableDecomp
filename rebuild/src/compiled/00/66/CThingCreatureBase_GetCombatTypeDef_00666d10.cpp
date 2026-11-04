#include "fable_creature_combat.h"
const CCombatTypeDef& CThingCreatureBase::GetCombatTypeDef() const
{
    return *GetPCombatTypeDef();
}
