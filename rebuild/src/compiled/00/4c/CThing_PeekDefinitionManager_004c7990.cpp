#include "fable_world.h"
#include "fable_thing_manager.h"
const CGameDefinitionManager& CThing::PeekDefinitionManager() const
{
    return PeekPCurrentThingManager()->PeekDefinitionManager();
}
