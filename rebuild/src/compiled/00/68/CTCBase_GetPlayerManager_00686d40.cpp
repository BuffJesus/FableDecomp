#include "fable_world.h"
#include "fable_thing_manager.h"
CPlayerManager& CTCBase::GetPlayerManager()
{
    return CThing::GetPCurrentThingManager()->GetPlayerManager();
}
