// CMainGameComponent::GetGameTimeOfNextPresentCompletion @ 0x004166E2
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"

double CMainGameComponent::GetGameTimeOfNextPresentCompletion() const
{
    const CVsyncCallbackTimerInfo info =
        g_FableDisplayManager_013B8390->GetVsyncCallbackTimer().GetInfo();

    double presentTime = info.GPUTimeStamp +
        (info.CPUFrameCount - info.GPUFrameCount + 1) *
        g_FableDisplayManager_013B8390->PresentationIntervalTime;

    const double currentTime = GFGetTime();
    if (presentTime < currentTime)
        presentTime = currentTime;
    if (UseOldInterpolationTiming)
        presentTime = currentTime;

    return presentTime - GameStartTime;
}