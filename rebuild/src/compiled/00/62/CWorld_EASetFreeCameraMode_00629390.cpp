#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
void CWorld::EASetFreeCameraMode(const CGameEvent* event)
{
    CGameEventDataPackage event_data;
    event->GetDataPackage(&event_data);
    bool enable = event_data.GetUBYTE() == 1;
    CPlayer* player = GetPlayer(event->Player);
    if (enable) player->AddMode(PLAYER_MODE_CONTROL_CAMERA, false);
    else player->RemoveMode(PLAYER_MODE_CONTROL_CAMERA);
}
