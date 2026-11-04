// CMainGameComponent::UpdateForcedTickSpeed @ 0x00417747
// (FableWin ?UpdateForcedTickSpeed@CMainGameComponent@@QAEXXZ)
//
// The fixed-tick path Run takes when ForceUpdateTick is set: run at most one
// game turn, then render `ratio` interpolated frames (desired framerate over
// game framerate), sleeping out each frame's slice, and finally sleep off
// whatever is left of the game frame. A tick that overruns its frame counts
// as a failed update.
#include "fable_game.h"
#include "fable_display_engine.h"
#include "fable_fpu.h"
#include "fable_gfuninitialise.h"
#include "fable_world.h"

#pragma optimize("s", on)   // retail TU is size-optimised

extern "C" __declspec(dllimport) void FABLE_STDCALL Sleep(unsigned long milliseconds);

// FableWin ?NoLookups@CThingSearchTools@@2KA (0x013B89A8).
class CThingSearchTools
{
public:
    static unsigned long NoLookups;
};

// FableWin ?GWorldFrame@@3KA (0x013B7D70).
extern unsigned long g_WorldFrame_013B7D70;

void CMainGameComponent::UpdateForcedTickSpeed()
{
    float speedMultiplier = ForceUpdateTickSpeedMultipler;
    float desiredFramerate = ForceUpdateTickSpeedDesiredFramerate;
    float gameFramerate = GetConstantFPS() / speedMultiplier;
    float desiredGameFramerate = desiredFramerate / speedMultiplier;
    float framesPerTurn = desiredGameFramerate / gameFramerate;
    float turnTime = 1.0f / gameFramerate;

    if (!GFGetSystemManager()->IsApplicationActive())
        return;

    double startTime = GFGetTime();
    double timeUsed = 0.0;
    CThingSearchTools::NoLookups = 0;
    GFFPUClear();
    GFFPUSetDoublePrecision();
    GFFPUSetRoundingNearest();

    if (Client.GetGameEventPackageSet(&EventPackageSet))
    {
        PWorld->UpdateInputRelatedInterpolation();
        GetInputs();
        SignalFrameUpdate();
        UpdateFromEventPackageSet(&EventPackageSet);
        NoRenderFramesSinceLastGameUpdate = 0;
        WorldUpdateTurn = true;
    }
    g_WorldFrame_013B7D70 = CWorld::GetFrame();

    double updateTime = GFGetTime() - startTime;
    timeUsed += updateTime;

    if (CWorld::GetFrame() != 0)
    {
        CThing::SetCurrentThingManager(PWorld->GetThingManager());

        float interpolate = 0.0f;
        double renderSlice = (turnTime - timeUsed) / framesPerTurn;
        long noRenders = (long)framesPerTurn;
        if (noRenders < 1)
            noRenders = 1;

        for (long i = 0; i < noRenders; ++i)
        {
            double renderStart = GFGetTime();
            CInterpolationInfo info;
            RoughFPSCounter.NewFrame();
            info.Last = LastInterpolationInfo.Current;
            info.PausedLast = LastInterpolationInfo.PausedCurrent;
            info.WFServerCurrentTime = GetCurrentServerFrame();
            info.Current.GTPredictedTimeSinceLastRenderFrame = (float)GetPredictedTimeSinceLastRenderFrame();
            info.Current.WFInterpolate = interpolate;
            info.PausedCurrent = info.Current;
            LastInterpolationInfo = info;
            PWorld->RenderUpdate(&info);
            LastRenderFrameStartTime = GetCurrentGameTime();
            PDisplayEngine->Render(&info, WorldUpdateTurn, true);
            ++NoRenderFramesSinceLastGameUpdate;
            interpolate += 1.0f / framesPerTurn;

            double renderTime = GFGetTime() - renderStart;
            LastFrameRenderDuration = renderTime;
            timeUsed += renderTime;
            double sleepTime = renderSlice - renderTime;
            long sleepMs = (long)(sleepTime * 1000.0);
            if (sleepMs < 0)
                sleepMs = 0;
            else if (sleepMs > 1000)
                sleepMs = 1000;
            Sleep(sleepMs);
            timeUsed += sleepTime;
        }

        if (timeUsed > turnTime)
            ++ForceUpdateNoFailedUpdates;
    }

    double remaining = turnTime - timeUsed;
    long sleepMs = (long)(remaining * 1000.0);
    if (sleepMs < 0)
        sleepMs = 0;
    else if (sleepMs > 1000)
        sleepMs = 1000;
    Sleep(sleepMs);
}