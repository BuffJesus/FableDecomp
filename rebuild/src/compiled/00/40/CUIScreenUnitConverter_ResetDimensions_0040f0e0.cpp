// NUISystem::CUIScreenUnitConverter::ResetDimensions @ 0x0040F0E0 (43 bytes).
// FableWin ?ResetDimensions@CUIScreenUnitConverter@NUISystem@@SIXXZ: cache
// the display manager's render-target size as the UI back-buffer size.
#include "fable_nui_manager.h"
#include "fable_gfuninitialise.h"
#include "fable_surface.h"

void FABLE_FASTCALL NUISystem::CUIScreenUnitConverter::ResetDimensions()
{
    C2DExtentsI dimensions =
        GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions();
    BackBufferWidth = (float)dimensions.X;
    BackBufferHeight = (float)dimensions.Y;
}
