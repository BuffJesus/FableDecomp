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

void CMainGameComponent::SetDisplayMode(unsigned long width, unsigned long height, unsigned char depth)
{
    PreChangeResolution();
    g_FableDisplayManager_013B8390->SetDisplayMode(C2DExtentsI(width, height), depth, SURFACE_MULTISAMPLE_NONE, 0);
    PostChangeResolution(
        g_FableDisplayManager_013B8390->GetBackBuffer().GetWidth(),
        g_FableDisplayManager_013B8390->GetBackBuffer().GetHeight(),
        g_FableDisplayManager_013B8390->GetBackBuffer().GetPixelFormat().GetColourDepth());
}