// CMainGameComponent::Render @ 0x00417001 (protected virtual, vtable slot 7)
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_world.h"
#include "engine/CBulletTimeManager.h"
#include "engine/CDisplayEngine.h"

void CMainGameComponent::Render()
{
    CInterpolationInfo info;

    CThing::SetCurrentThingManager(PWorld->GetThingManager());

    if (GFGetSystemManager()->IsApplicationActive() && AllowRender && CWorld::GetFrame() > 1)
    {
        if (g_ForceRenderFramesPerSec_013B8630 > 0)
        {
            if (g_ForceRenderFramesPerSec_013B8630 < ConstantFPS)
                g_ForceRenderFramesPerSec_013B8630 = ConstantFPS;
            ForceRender(g_ForceRenderFramesPerSec_013B8630);
            Rendered = true;
        }
        else
        {
            if (!PWorld->Paused)
            {
                info.Last = LastInterpolationInfo.Current;
                info.PausedLast = LastInterpolationInfo.PausedCurrent;
                info.WFServerCurrentTime = CurrentServerFrame;
                info.Current.WFInterpolate = (float)GFLimitReturn(
                    ConvertGTToWF(GetGameTimeOfNextPresentCompletion()) - CurrentServerFrame, 0.0, 1.0);
                info.Current.GTPredictedTimeSinceLastRenderFrame = (float)GetPredictedTimeSinceLastRenderFrame();
                info.PausedCurrent = info.Current;
                info.BulletTimeCurrent = info.Current;

                CBulletTimeManager* bulletTime = PWorld->PBulletTimeManager.get();
                if (bulletTime->IsBulletTimeActive())
                {
                    info.BulletTimeCurrent.WFInterpolate =
                        bulletTime->ConvertInterpolateToBulletTimeInterpolate(info.Current.WFInterpolate);
                    info.BulletTimeLast.WFInterpolate =
                        PWorld->PBulletTimeManager->ConvertInterpolateToBulletTimeInterpolate(info.Last.WFInterpolate);
                }
            }
            else
            {
                info.Last = LastInterpolationInfo.Last;
                info.Current = LastInterpolationInfo.Current;
                info.PausedLast = LastInterpolationInfo.PausedCurrent;
                info.BulletTimeCurrent = LastInterpolationInfo.BulletTimeCurrent;
                info.WFServerCurrentTime = CurrentServerFrame;
                info.PausedCurrent.WFInterpolate = (float)GFLimitReturn(
                    ConvertGTToWF(GetGameTimeOfNextPresentCompletion()) - CurrentServerFrame, 0.0, 1.0);
                info.PausedCurrent.GTPredictedTimeSinceLastRenderFrame = (float)GetPredictedTimeSinceLastRenderFrame();
            }

            LastInterpolationInfo = info;
            PWorld->RenderUpdate(&info);

            if (!Client.IsFreeToRender())
                ++DebugNoFramesUnableToRender;

            if (AlwaysUpdateGameFrame || Client.IsFreeToRender())
            {
                const double renderStartTime = GetCurrentGameTime();
                LastRenderFrameStartTime = GetCurrentGameTime();
                PDisplayEngine->Render(&info, WorldUpdateTurn, true);
                const double renderEndTime = GetCurrentGameTime();
                LastFrameRenderDuration = renderEndTime - renderStartTime;
                ++NoRenderFramesSinceLastGameUpdate;
                DebugNoFramesUnableToRender = 0;
                Rendered = true;
            }
        }
    }

    g_RenderFrame_013B7D6C = PDisplayEngine->Frame;
}