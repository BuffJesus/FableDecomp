// CMainGameComponent::Run @ 0x004189C2 (vtable slot 2).
// The main game's lifetime: start the clocks, optionally begin recording or
// replaying input, set the presentation interval, apply the user profile and
// start the first engine scene, then run one update per frame until the
// component quits; finally hand back the next component.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_system_manager.h"
#include "fable_definition_manager.h"
#include "fable_user_profile.h"
#include "fable_memory_allocator.h"
#include "fable_iengine.h"
#include "fable_display_engine.h"
#include "fable_player_manager.h"
#include "fable_world.h"
#include "fable_world_map.h"

// FableWin ?GFLogMemoryUse@@YIXXZ (0x00415E85).
void FABLE_FASTCALL GFLogMemoryUse();

// Command-line/debug globals (retail addresses; names not in the PDBs).
extern bool g_FableLoadInputEvents_013B85F6;
extern bool g_FableSaveInputEvents_013B85F5;
extern CWideString g_FableInputEventsFileName_013B865C;
extern bool g_FablePresentationIntervalOverride_013B8628;

bool CMainGameComponent::Run(CGameComponent** nextComponent)
{
    Client.WaitForSync();
    GameStartTime = GFGetTime();
    Running = true;
    Quit = false;
    LastRenderFrameStartTime = GetCurrentGameTime();

    if (g_FableLoadInputEvents_013B85F6)
        BeginInputLoading(g_FableInputEventsFileName_013B865C);
    else if (g_FableSaveInputEvents_013B85F5)
        BeginInputSaving(g_FableInputEventsFileName_013B865C);

    if (g_FablePresentationIntervalOverride_013B8628)
    {
        g_FableDisplayManager_013B8390->ChangePresentationInterval(
            g_FablePresentationIntervalOverride_013B8628, 1, true);
    }
    else
    {
        CDefPointer<const CRegionDef> regionDef =
            PWorld->PeekWorldMap().GetRegion(PWorld->PeekWorldMap().GetLoadedRegion()).GetPRegionDef();
    }

    CUserProfileManager::Get().Apply();
    PDisplayEngine->Engine3D->StartNewScene(12, 20.0f);
    CMemoryAllocator::Get()->ConsolidateFreeAreas();
    CMemoryAllocator::Get()->ClearPerformanceStats();

    while (!Quit)
    {
        CThreadedFileManager::ProcessDirtyDiscError();
        CurrentFrameStartGameTime = GetCurrentGameTime();

        const ESystemReturn result = GFGetSystemManager()->Update();
        if (result == ESR_QUIT)
        {
            Quit = true;
        }
        else if (result == ESR_OK)
        {
            if (GFGetSystemManager()->GetProfileManager())
                GFGetSystemManager()->GetProfileManager()->Update();

            if (ForceUpdateTick)
            {
                UpdateForcedTickSpeed();
            }
            else
            {
                RoughFPSCounter.NewFrame();
                UpdateRegular();
            }
        }

        const double frameStartTime = CurrentFrameStartGameTime;
        UpdateAverageFrameDuration(GetCurrentGameTime() - frameStartTime);
        GFLogMemoryUse();
        CGameDefinitionManager::Get().ScrambleDefs(true);
    }

    Running = false;
    *nextComponent = NextComponentToRun;
    Shutdown();
    g_FableRetiredGameComponent_013B7D58 = this;
    return *nextComponent != 0;
}