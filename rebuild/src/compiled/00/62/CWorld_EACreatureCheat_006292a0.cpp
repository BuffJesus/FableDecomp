// CWorld::EACreatureCheat @ 006292A0; PDB method names, primary creature ancestry.
#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"
void CWorld::EACreatureCheat(const CGameEvent* event)
{
    CThingPlayerCreature* creature = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (creature)
        creature->Kill(true);
}
