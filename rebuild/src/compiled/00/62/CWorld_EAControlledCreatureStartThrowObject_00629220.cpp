// CWorld::EAControlledCreatureStartThrowObject @ 00629220; FableWin signature and retail call chain.
#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"

void CWorld::EAControlledCreatureStartThrowObject(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    player->AddMode(PLAYER_MODE_FIRST_PERSON, false);
}
