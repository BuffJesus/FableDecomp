// CWorld::EAToggleKillEverythingMode @ 00629570; FableWin inline player bool accessors.
#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
void CWorld::EAToggleKillEverythingMode(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    player->SetKillEverythingMode(!player->IsKillEverythingMode());
}
