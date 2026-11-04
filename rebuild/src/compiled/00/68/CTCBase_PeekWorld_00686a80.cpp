#include "fable_world.h"
#include "fable_thing_manager.h"
const CWorld& CTCBase::PeekWorld() const
{
    return CThing::PeekPCurrentThingManager()->PeekWorld();
}
