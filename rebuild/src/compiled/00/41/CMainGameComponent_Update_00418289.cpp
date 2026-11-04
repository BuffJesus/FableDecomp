// CMainGameComponent::Update @ 0x00418289 (FableWin ?Update@CMainGameComponent@@MAEXXZ, vtable slot 5)
//
// One game-loop tick: pin the x87 state, honour the debug frame delay, pause
// the world (a "pause" game event) while a disconnected controller blocks an
// interactive subtitle, then pull this frame's event-package set either from
// the saved-input file or from the network client and run it.
#include "fable_game.h"
#include "fable_fpu.h"
#include "fable_game_event.h"
#include "fable_nui_manager.h"
#include "fable_player_gui.h"
#include "fable_system_manager.h"
#include "fable_world.h"

#pragma optimize("s", on)   // retail TU is size-optimised

extern "C" __declspec(dllimport) void FABLE_STDCALL Sleep(unsigned long milliseconds);

// FableWin ?GDelayEachFrameMS@@3JA (0x013B8610) and ?GWorldFrame@@3KA (0x013B7D70).
extern long g_DelayEachFrameMS_013B8610;
extern unsigned long g_WorldFrame_013B7D70;

void CMainGameComponent::Update()
{
    // FableWin static local BRequireUnpause (0x013B86AC).
    static bool BRequireUnpause;

    GFFPUClear();
    GFFPUSetDoublePrecision();
    GFFPUSetRoundingNearest();

    if (g_DelayEachFrameMS_013B8610 > 0)
        Sleep(g_DelayEachFrameMS_013B8610);

    CSubtitleRenderer& subtitles = CPlayerGui::Get().GetSubtitleRenderer();
    if (IsControllerDisconnected() && subtitles.IsInInteractiveState())
    {
        if (!NUISystem::CManager::GetInstance()->HasErrorMessage())
        {
            if (!PWorld->IsPaused())
            {
                CGameEvent event(9);
                event.AddUBYTE(1);
                AddGameEvent(event);
                BRequireUnpause = true;
            }
            else
            {
                BRequireUnpause = false;
            }
            ShowErrorMessage(EMT_CONTROLLER_DISCONNECT);
        }
    }
    else if (NUISystem::CManager::GetInstance()->HasErrorMessage())
    {
        NUISystem::CManager::GetInstance()->ClearErrorMessage();
        if (BRequireUnpause)
        {
            CGameEvent event(9);
            event.AddUBYTE(0);
            AddGameEvent(event);
            BRequireUnpause = false;
        }
    }

    TimePassedSinceLastUpdate = GFGetTime() - LastUpdateTime;
    LastUpdateTime = GFGetTime();
    WorldUpdateTurn = false;

    if (LoadingEventPackages)
    {
        if (IsTimeForServerUpdate(CurrentServerFrame))
        {
            if (GetEventPackageSetFromSave(&EventPackageSet) == true)
            {
                UpdateFromEventPackageSet(&EventPackageSet);
                NoRenderFramesSinceLastGameUpdate = 0;
                WorldUpdateTurn = true;
            }
            SignalFrameUpdate();
        }
    }
    else
    {
        if (Client.IsLocal() && CurrentServerFrame > 1
            && !g_FableInputManager_013B8388->IsLoadingEvents()
            && !g_FableInputManager_013B8388->IsSavingEvents())
        {
            long frameDifference = GetFrameDifferenceFromCurrent(GetCurrentServerFrame());
            if (g_ForceRenderFramesPerSec_013B8630 == 0 && frameDifference > 15)
            {
                CurrentServerFrame += frameDifference;
                AllowRender = false;
            }
        }

        if (Client.GetGameEventPackageSet(&EventPackageSet))
        {
            PWorld->UpdateInputRelatedInterpolation();
            GetInputs();
            SignalFrameUpdate();
            if (SavingEventPackages)
                AddEventPackageSetToSave(&EventPackageSet);
            UpdateFromEventPackageSet(&EventPackageSet);
            NoRenderFramesSinceLastGameUpdate = 0;
            WorldUpdateTurn = true;
        }
    }

    g_WorldFrame_013B7D70 = CWorld::GetFrame();
}