#include "fable_world.h"
#include "fable_game_event.h"

void CWorld::EASetSlowMotion(const CGameEvent* event)
{
    CGameEventDataPackage package;
    event->GetDataPackage(&package);
    SlowMotion=package.GetUBYTE()==1;
}
