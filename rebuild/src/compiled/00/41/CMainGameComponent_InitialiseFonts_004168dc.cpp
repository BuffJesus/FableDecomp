// CMainGameComponent::InitialiseFonts @ 0x004168DC
// FableWin.pdb: ?InitialiseFonts@CMainGameComponent@@IAEXXZ.
// Fetch the cut-scene font from the font manager (also re-run by
// OnPostDeviceReset 0x004175DA).
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_progress_display.h"
#include "fable_system_manager.h"

void CMainGameComponent::InitialiseFonts()
{
    NProgressDisplay::DisplayProgress("Init Fonts", -1.0f, false, false);
    PCutSceneMainFont = g_FableFontManager_013B838C->GetFont("ENG_ARIAL_18");
}