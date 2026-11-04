// CMainGameComponent::AppReinitFunc @ 0x004167DA (88 bytes).
// FableWin ?AppReinitFunc@CMainGameComponent@@KIXPAX@Z (protected static
// fastcall). Init registers it with the system manager, passing the
// component as context; after an application reinit it re-runs the
// resolution-change pair against the current back buffer.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_display_engine.h"
#include "fable_surface.h"

// ?PreChangeResolution@CMainGameComponent@@QAEXXZ; inline in retail.
inline void CMainGameComponent::PreChangeResolution()
{
    PDisplayEngine->PreChangeResolution();
}

void FABLE_FASTCALL CMainGameComponent::AppReinitFunc(void* context)
{
    CMainGameComponent* component = static_cast<CMainGameComponent*>(context);
    component->PreChangeResolution();
    component->PostChangeResolution(
        g_FableDisplayManager_013B8390->GetBackBuffer().GetWidth(),
        g_FableDisplayManager_013B8390->GetBackBuffer().GetHeight(),
        g_FableDisplayManager_013B8390->GetBackBuffer().GetPixelFormat().GetColourDepth());
}
