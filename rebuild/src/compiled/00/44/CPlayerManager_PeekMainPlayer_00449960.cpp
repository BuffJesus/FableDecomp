// CPlayerManager::GetMainPlayer @ 00449960; FableWin ABI and retail STLport layout.
#include "fable_player_manager.h"

const CPlayer& CPlayerManager::GetMainPlayer() const
{
    return GetPlayer(MainPlayer);
}
