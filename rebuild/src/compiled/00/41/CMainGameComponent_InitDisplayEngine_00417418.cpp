// CMainGameComponent::InitDisplayEngine @ 0x00417418 (217 bytes).
// FableWin ?InitDisplayEngine@CMainGameComponent@@IAEXXZ. Describes the back
// buffer, definitions, players, world and graphics to a new CDisplayEngine,
// reports progress, then initialises it.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_definition_manager.h"
#include "fable_display_engine.h"
#include "fable_player_manager.h"
#include "fable_progress_display.h"
#include "fable_surface.h"
#include "fable_world.h"

// ?PeekDefinitionManager@CMainGameComponent@@QBEABVCGameDefinitionManager@@XZ;
// inline in retail.
inline const CGameDefinitionManager& CMainGameComponent::PeekDefinitionManager() const
{
    return CGameDefinitionManager::Get();
}

void CMainGameComponent::InitDisplayEngine()
{
    CDisplayEngineInit init;
    init.World = PWorld.get();
    init.Height = g_FableDisplayManager_013B8390->GetBackBuffer().GetHeight();
    init.Width = g_FableDisplayManager_013B8390->GetBackBuffer().GetWidth();
    init.PixelFormat = g_FableDisplayManager_013B8390->GetBackBuffer().GetPixelFormat();
    init.DefinitionManager = &PeekDefinitionManager();
    init.PlayerManager = PPlayerManager.get();
    init.Component = this;
    init.MainGraphicBank = PMainGraphicBank.Get();
    init.MeshBank = PWorld->GetMeshBank();

    PDisplayEngine.reset(new CDisplayEngine(init));

    {
        CCharString text("Init Display Engine: Calling Init");
        NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
    }

    PDisplayEngine->Init();
}