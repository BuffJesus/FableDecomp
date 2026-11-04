// CWorld::EAControlledCreatureDeactivateZTarget @ 00629240; FableWin signature and retail call chain.
#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"

void CWorld::EAControlledCreatureDeactivateZTarget(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    player->RemoveMode(PLAYER_MODE_Z_TARGET);
}
