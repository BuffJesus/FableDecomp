#include "fable_world.h"
#include "fable_thing_manager.h"
CWorld& CTCBase::GetWorld() const
{
    return CThing::GetPCurrentThingManager()->GetWorld();
}
