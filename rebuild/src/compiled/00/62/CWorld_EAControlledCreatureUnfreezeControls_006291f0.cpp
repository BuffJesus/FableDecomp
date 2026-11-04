// CWorld::EAControlledCreatureUnfreezeControls @ 006291F0; FableWin signature and retail call chain.
#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"

void CWorld::EAControlledCreatureUnfreezeControls(const CGameEvent* event)
{
    GetPlayer(event->Player)->UnfreezeControls();
}
