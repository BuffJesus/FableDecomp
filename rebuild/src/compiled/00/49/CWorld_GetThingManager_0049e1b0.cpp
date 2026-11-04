#include "fable_world.h"

CThingManager& CWorld::GetThingManager()
{
    return *PThingManager;
}
