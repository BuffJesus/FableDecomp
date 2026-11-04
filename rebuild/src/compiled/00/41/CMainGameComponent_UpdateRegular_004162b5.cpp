// CMainGameComponent::UpdateRegular @ 0x004162B5 (FableWin ?UpdateRegular@CMainGameComponent@@QAEXXZ)
//
// The per-frame path Run takes when ForceUpdateTick is off: while the
// application is active, reset the thing-search lookup counter, time the
// Update() virtual into ProfileUpdateTime, then time the render step into
// ProfileRenderTime. Render() is skipped only while the progress display is
// up and the 3D engine reports itself preloaded: it runs unless both
// NProgressDisplay::IsActive() and Engine3D->IsPreloaded() hold.
#pragma optimize("s", on)   // retail TU is size-optimised (and [mem],0 / push ecx; frame)
#include "fable_game.h"
#include "fable_gfuninitialise.h"
#include "fable_iengine.h"
#include "fable_display_engine.h"
#include "fable_progress_display.h"

// FableWin ?NoLookups@CThingSearchTools@@2KA (0x013B89A8).
class CThingSearchTools
{
public:
    static unsigned long NoLookups;
};

void CMainGameComponent::UpdateRegular()
{
    if (GFGetSystemManager()->IsApplicationActive())
    {
        CThingSearchTools::NoLookups = 0;

        double start = GFGetTime();
        Update();
        ProfileUpdateTime = GFGetTime() - start;

        start = GFGetTime();
        if (!NProgressDisplay::IsActive() || !PDisplayEngine->Engine3D->IsPreloaded())
            Render();
        ProfileRenderTime = GFGetTime() - start;
    }
}