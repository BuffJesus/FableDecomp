// CMainGameComponent::IsTimeForServerUpdate @ 0x0041674A (93 bytes).
// FableWin ?IsTimeForServerUpdate@CMainGameComponent@@QBE_NJ@Z. Never while
// the component is not running. A movie-recording replay (render only while
// recording, events loading, not yet recording) and the AlwaysUpdateGameFrame
// override always update; otherwise it is time once the next present lands
// more than one world frame past `frame`.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_system_manager.h"

bool CMainGameComponent::IsTimeForServerUpdate(long frame) const
{
    if (Running)
    {
        if (g_RenderOnlyWhenRecordingMovie_013B860C
            && g_FableInputManager_013B8388->IsLoadingEvents()
            && !g_RecordMovie_013B8629)
            return true;
        if (AlwaysUpdateGameFrame)
            return true;
        float framesAhead = ConvertGTToWF(GetGameTimeOfNextPresentCompletion()) - frame;
        if (framesAhead > 1.0f)
            return true;
    }
    return false;
}
