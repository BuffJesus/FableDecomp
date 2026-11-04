// lib_global_tools.cpp, retail 0x009E23A0 / 0x009E23F0 (Ego_r ?GFXToY@@YIMM@Z,
// ?GFYToX@@YIMM@Z): a horizontal length in render-target pixels as the same
// fraction of the height, and back. Each fetches the display manager twice;
// GFXToY stores the scaled length back into x inside the one expression.
#include "fable_display_viewport.h"
#include "fable_system_manager_state.h"
#include "fable_virtual_pixels.h"
CSystemManager* FABLE_FASTCALL GFGetSystemManager();

// Retail 009E23A0.
float FABLE_FASTCALL GFXToY(float x)
{
    CDisplayManager* width_display = GFGetSystemManager()->PDisplayManager.Data;
    CDisplayManager* height_display = GFGetSystemManager()->PDisplayManager.Data;
    return (x *= width_display->GetRenderTargetDimensions().X) / height_display->GetRenderTargetDimensions().Y;
}

// Retail 009E23F0.
float FABLE_FASTCALL GFYToX(float y)
{
    CDisplayManager* height_display = GFGetSystemManager()->PDisplayManager.Data;
    CDisplayManager* width_display = GFGetSystemManager()->PDisplayManager.Data;
    return y * height_display->GetRenderTargetDimensions().Y / width_display->GetRenderTargetDimensions().X;
}
