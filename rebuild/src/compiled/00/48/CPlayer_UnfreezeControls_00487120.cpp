// CPlayer::UnfreezeControls @ 00487120; FableWin name and retail caller.
#include "fable_player_manager.h"

void CPlayer::UnfreezeControls()
{
    RemoveMode(PLAYER_MODE_FREEZE_CONTROLS);
    PlayerInterface->ResumeAllInputProcesses();
}
