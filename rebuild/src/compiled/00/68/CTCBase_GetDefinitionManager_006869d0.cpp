#include "fable_world.h"
const CGameDefinitionManager& CTCBase::GetDefinitionManager() const
{
    return GetParentThing().PeekDefinitionManager();
}
