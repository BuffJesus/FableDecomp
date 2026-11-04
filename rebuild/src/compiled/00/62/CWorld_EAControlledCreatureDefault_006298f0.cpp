#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
void CWorld::EAControlledCreatureDefault(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    if (IS_THING_ALIVE(player->GetPControlledCreature()) && player->GetCurrentMode() != PLAYER_MODE_CONTROL_CREATURE)
        player->AddMode(PLAYER_MODE_CONTROL_CREATURE, true);
}
