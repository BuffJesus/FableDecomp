#include "fable_world.h"

const CThingManager& CWorld::PeekThingManager() const
{
    return *PThingManager;
}
