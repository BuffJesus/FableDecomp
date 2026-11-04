// CMainGameComponent::InitWorld @ 0x0041735A (190 bytes).
// FableWin ?InitWorld@CMainGameComponent@@IAEXXZ. Builds the world from a
// CWorldInit naming this component, the player manager and the game
// definition manager; opens the event-package replay/record files when the
// command line asked for them; reports progress; then initialises the world
// (CWorld vtable slot 9).
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_definition_manager.h"
#include "fable_player_manager.h"
#include "fable_progress_display.h"
#include "fable_world.h"

void CMainGameComponent::InitWorld()
{
    CWorldInit init;
    init.Component = this;
    init.PlayerManager = PPlayerManager.get();
    init.DefinitionManager = &CGameDefinitionManager::Get();

    PWorld.reset(new CWorld(init));

    if (g_LoadEventPackages_013B85F8)
        InitEventPackageLoading(g_EventPackageFileName_013B8660);
    if (g_SaveEventPackages_013B85F7)
        InitEventPackageSaving(g_EventPackageFileName_013B8660);

    {
        CCharString text("Init World Init");
        NProgressDisplay::DisplayProgress(text, -1.0f, false, false);
    }

    PWorld->Init();
}
