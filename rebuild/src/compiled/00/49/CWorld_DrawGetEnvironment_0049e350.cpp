#include "fable_world.h"

const CIDrawEnvironment* CWorld::DrawGetEnvironment() const
{
    return reinterpret_cast<const CIDrawEnvironment*>(PEnvironment.get());
}
