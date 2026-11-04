#include "fable_thing_components.h"
#include "fable_creature_combat.h"
bool CTCCombat::IsAbleToBeKnockedDown() const
{
    if (GetParentCreature().GetCombatTypeDef().CanBeKnockedDown)
        return true;
    return false;
}
