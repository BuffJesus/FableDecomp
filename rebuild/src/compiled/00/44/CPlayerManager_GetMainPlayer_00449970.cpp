// CPlayerManager::GetMainPlayer @ 00449970; FableWin ABI and retail STLport layout.
#include "fable_player_manager.h"

CPlayer& CPlayerManager::GetMainPlayer()
{
    return GetPlayer(MainPlayer);
}
