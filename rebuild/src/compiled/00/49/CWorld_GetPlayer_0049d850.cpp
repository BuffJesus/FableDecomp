// CWorld::GetPlayer @ 0049D850; FableWin signature and retail call chain.
#include "fable_world.h"
#include "fable_game_event.h"
#include "fable_player_manager.h"

CPlayer* CWorld::GetPlayer(unsigned long number)
{
    return &PlayerManager->GetPlayer(number);
}
