#include "fable_display_viewport.h"
#include "fable_system_manager_state.h"
#include "fable_virtual_pixels.h"
#include "fable_display_geometry.h"
CSystemManager* FABLE_FASTCALL GFGetSystemManager();
static __forceinline float virtualX(float value)
{
    CDisplayManager* display=GFGetSystemManager()->PDisplayManager.Data;
    return (float)(value*(float)g_FableVirtualWidth_013961E8/display->GetRenderTargetDimensions().X);
}
static __forceinline float virtualY(float value)
{
    CDisplayManager* display=GFGetSystemManager()->PDisplayManager.Data;
    return (float)(value*(float)g_FableVirtualHeight_013961F0)/display->GetRenderTargetDimensions().Y;
}
void FABLE_FASTCALL GFABoxToVBox(const C2DBoxF& input,C2DBoxF& output)
{
    output.left=virtualX(input.left);
    output.top=virtualY(input.top);
    output.right=virtualX(input.right);
    output.bottom=virtualY(input.bottom);
}

// Retail 009E20F0 (Ego_r ?GFABoxToVBox@@YIXABVC2DBoxI@@AAV1@@Z): the integer box.
static __forceinline long boxVirtualX(long value)
{
    CDisplayManager* display=GFGetSystemManager()->PDisplayManager.Data;
    return (long)(value*(float)g_FableVirtualWidth_013961E8/display->GetRenderTargetDimensions().X);
}
static __forceinline long boxVirtualY(long value)
{
    CDisplayManager* display=GFGetSystemManager()->PDisplayManager.Data;
    return (long)(value*(float)g_FableVirtualHeight_013961F0/display->GetRenderTargetDimensions().Y);
}
void FABLE_FASTCALL GFABoxToVBox(const C2DBoxI& input,C2DBoxI& output)
{
    output.left=boxVirtualX(input.left);
    output.top=boxVirtualY(input.top);
    output.right=boxVirtualX(input.right);
    output.bottom=boxVirtualY(input.bottom);
}
