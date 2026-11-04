#include "fable_world.h"
#include "fable_thing_manager.h"
const CPlayerManager& CTCBase::PeekPlayerManager() const
{
    return CThing::PeekPCurrentThingManager()->PeekPlayerManager();
}
