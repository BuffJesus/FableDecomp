#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
void CWorld::EAControlledCreatureBlockEnd(const CGameEvent* event)
{
    CPlayer* player = GetPlayer(event->Player);
    CThingPlayerCreature* creature = player->GetPControlledCreature();
    if (player->IsMainPlayer() && IS_THING_ALIVE(creature))
    {
        CTCCombat* combat;
        if (creature->GetTC(&combat))
            combat->UnsetBlocking();
    }
}
