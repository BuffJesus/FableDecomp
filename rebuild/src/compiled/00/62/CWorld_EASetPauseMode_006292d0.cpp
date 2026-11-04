#include "fable_world.h"
#include "fable_game_event.h"

void CWorld::EASetPauseMode(const CGameEvent* event)
{
    CGameEventDataPackage package;
    event->GetDataPackage(&package);
    SetAsPaused(package.GetUBYTE()==1);
}
