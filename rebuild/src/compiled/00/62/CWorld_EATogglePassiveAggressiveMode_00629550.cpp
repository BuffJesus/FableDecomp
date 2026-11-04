// CWorld::EATogglePassiveAggressiveMode @ 00629550; FableWin signature and retail call chain.
#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"

void CWorld::EATogglePassiveAggressiveMode(const CGameEvent* event)
{
    GetPlayer(event->Player)->ToggleAggressiveMode();
}
