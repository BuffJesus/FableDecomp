// CWorld::EAOpenPCMSNChatMenu @ 0062C6E0; FableWin signature and retail call chain.
#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"

void CWorld::EAOpenPCMSNChatMenu(const CGameEvent* event)
{
    GetPlayer(event->Player)->GetPControlledCreature();
}
