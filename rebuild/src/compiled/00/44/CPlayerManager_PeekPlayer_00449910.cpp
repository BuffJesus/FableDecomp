// CPlayerManager::GetPlayer @ 00449910; FableWin ABI and retail STLport layout.
#include "fable_player_manager.h"

const CPlayer& CPlayerManager::GetPlayer(long number) const
{
    long count = static_cast<long>(Players.size());
    for (long i = 0; i < count; ++i)
        if (Players[i]->GetNumber() == number)
            return *Players[i];
    return *Players[PlayerNeutral];
}
