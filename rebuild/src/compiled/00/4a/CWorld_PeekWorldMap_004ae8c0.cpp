#include "fable_world.h"

const CWorldMap& CWorld::PeekWorldMap() const
{
    return *PWorldMap;
}
