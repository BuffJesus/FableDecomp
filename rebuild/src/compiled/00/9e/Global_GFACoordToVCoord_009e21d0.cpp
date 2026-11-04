// Retail 009E21D0: vertical numerator is rounded to float before division.
#include "fable_display_viewport.h"
#include "fable_system_manager_state.h"
#include "fable_virtual_pixels.h"
#include "fable_2d_vector.h"
#include "fable_display_geometry.h"
CSystemManager* FABLE_FASTCALL GFGetSystemManager();
static __forceinline float virtualX(float value)
{
    CDisplayManager* display = GFGetSystemManager()->PDisplayManager.Data;
    return (float)(value * (float)g_FableVirtualWidth_013961E8 / display->GetRenderTargetDimensions().X);
}
static __forceinline float virtualY(float value)
{
    CDisplayManager* display = GFGetSystemManager()->PDisplayManager.Data;
    return (float)(value * (float)g_FableVirtualHeight_013961F0) / display->GetRenderTargetDimensions().Y;
}
void FABLE_FASTCALL GFACoordToVCoord(const C2DVector& input, C2DVector& output)
{
    output.X = virtualX(input.X);
    output.Y = virtualY(input.Y);
}

// Retail 009E2240 (Ego_r ?GFACoordToVCoord@@YIXABVC2DCoordI@@AAV1@@Z): the
// integer form, each coordinate through the float scaling and truncated.
void FABLE_FASTCALL GFACoordToVCoord(const C2DCoordI& input, C2DCoordI& output)
{
    output.X = (long)virtualX((float)input.X);
    output.Y = (long)virtualY((float)input.Y);
}
