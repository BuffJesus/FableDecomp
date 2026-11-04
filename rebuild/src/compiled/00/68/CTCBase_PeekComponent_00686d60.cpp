#include "fable_world.h"
#include "fable_thing_manager.h"
const CMainGameComponent& CTCBase::PeekComponent() const
{
    return CThing::PeekPCurrentThingManager()->PeekComponent();
}
