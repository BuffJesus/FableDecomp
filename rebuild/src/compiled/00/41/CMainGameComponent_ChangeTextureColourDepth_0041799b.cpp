// CMainGameComponent::ChangeTextureColourDepth @ 0x0041799B (83 bytes).
// FableWin ?ChangeTextureColourDepth@CMainGameComponent@@UAEXJ@Z. Accepts a
// 16/24/32-bit texture depth the display manager supports; when it differs
// from the current one, records it, rebuilds the graphics formats and passes
// it on to the display engine and the world.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_display_engine.h"
#include "fable_world.h"

// Ego_r.pdb ?GTextureBitDepth@@3KA.
extern unsigned long g_FableTextureBitDepth_013B7D68;

void CMainGameComponent::ChangeTextureColourDepth(long depth)
{
    if ((depth == 16 || depth == 24 || depth == 32)
        && g_FableDisplayManager_013B8390->IsTextureBitDepthSupported(depth)
        && depth != (long)g_FableTextureBitDepth_013B7D68)
    {
        g_FableTextureBitDepth_013B7D68 = depth;
        InitGraphics();
        PDisplayEngine->ChangeTextureColourDepth(depth);
        PWorld->ChangeTextureColourDepth(depth);
    }
}
