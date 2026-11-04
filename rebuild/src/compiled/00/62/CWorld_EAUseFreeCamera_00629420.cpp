#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
void CWorld::EAUseFreeCamera(const CGameEvent* event)
{
    CGameEventDataPackage event_data;
    signed char player_number = event->Player;
    event->GetDataPackage(&event_data);
    bool enable = event_data.GetUBYTE() == 1;
    GetPlayer(player_number)->SetUsingFreeCamera(enable);
}
