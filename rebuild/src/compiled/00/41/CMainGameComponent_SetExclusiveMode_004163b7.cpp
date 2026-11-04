// CMainGameComponent::SetExclusiveMode @ 0x004163B7 (104 bytes).
// FableWin ?SetExclusiveMode@CMainGameComponent@@QAEX_N@Z. Switches the
// system manager between exclusive (full-screen) and windowed mode, bracketed
// by the display engine's resolution-change hooks, and re-reads the back
// buffer's size and colour depth afterwards.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_display_engine.h"
#include "fable_gfuninitialise.h"
#include "fable_surface.h"

// ?PreChangeResolution@CMainGameComponent@@QAEXXZ; inline in retail.
inline void CMainGameComponent::PreChangeResolution()
{
    PDisplayEngine->PreChangeResolution();
}

void CMainGameComponent::SetExclusiveMode(bool exclusive)
{
    PreChangeResolution();
    GFGetSystemManager()->SetExclusiveMode(exclusive);
    PostChangeResolution(
        g_FableDisplayManager_013B8390->GetBackBuffer().GetWidth(),
        g_FableDisplayManager_013B8390->GetBackBuffer().GetHeight(),
        g_FableDisplayManager_013B8390->GetBackBuffer().GetPixelFormat().GetColourDepth());
}