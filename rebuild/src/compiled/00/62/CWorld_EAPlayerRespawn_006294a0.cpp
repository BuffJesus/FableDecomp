#include "fable_world.h"
#include "fable_game_event.h"
void CWorld::EAPlayerRespawn(const CGameEvent* event)
{
    CGameEventDataPackage event_data;
    event->GetDataPackage(&event_data);
    bool autoload = event_data.GetUBYTE() == 1;
    if (autoload) AutoLoadRespawn();
    else ResetToFrontEnd();
}
