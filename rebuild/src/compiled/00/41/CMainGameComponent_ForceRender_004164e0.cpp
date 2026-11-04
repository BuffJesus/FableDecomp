// CMainGameComponent::ForceRender @ 0x004164E0
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_world.h"
#include "engine/CDisplayEngine.h"

static const double DEFAULT_SERVER_FPS = 15.0;

void CMainGameComponent::ForceRender(long framesPerSecond)
{
    const long noFrames = (long)(framesPerSecond / DEFAULT_SERVER_FPS);
    const float frameTime = 1.0f / framesPerSecond;

    if (LastGameTurnForceRendered < CurrentServerFrame)
    {
        for (long i = 0; i < noFrames; ++i)
        {
            CInterpolationInfo info;
            info.Last = LastInterpolationInfo.Current;
            info.PausedLast = LastInterpolationInfo.PausedCurrent;
            info.WFServerCurrentTime = CurrentServerFrame;
            info.Current.WFInterpolate = i / (float)noFrames;
            info.Current.GTPredictedTimeSinceLastRenderFrame = (i + 1) * frameTime;
            info.PausedCurrent = info.Current;
            LastInterpolationInfo = info;

            PWorld->RenderUpdate(&info);

            const double renderStartTime = GetCurrentGameTime();
            LastRenderFrameStartTime = GetCurrentGameTime();
            PDisplayEngine->Render(&info, WorldUpdateTurn, true);
            const double renderEndTime = GetCurrentGameTime();
            LastFrameRenderDuration = renderEndTime - renderStartTime;
            ++NoRenderFramesSinceLastGameUpdate;
            LastGameTurnForceRendered = CurrentServerFrame;
        }
    }
}