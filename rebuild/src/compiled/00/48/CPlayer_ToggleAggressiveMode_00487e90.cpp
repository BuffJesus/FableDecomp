// CPlayer::ToggleAggressiveMode @ 00487E90; FableWin methods/global name.
#include "fable_player_manager.h"

void CPlayer::ToggleAggressiveMode()
{
    if (GUsePassiveAggressiveMode)
    {
        AggressiveMode = !IsAggressiveMode();
        SetUpControlsBasedOnAggressiveMode();
    }
}
