#include "fable_display_viewport.h"
#include "fable_system_manager_state.h"
#include "fable_virtual_pixels.h"
#include "fable_2d_vector.h"
#include "fable_display_geometry.h"
CSystemManager* FABLE_FASTCALL GFGetSystemManager();

float FABLE_FASTCALL GFVXToAX(float value)
{
    CDisplayManager* display = GFGetSystemManager()->PDisplayManager.Data;
    return (float)(display->GetRenderTargetDimensions().X * value / g_FableVirtualWidth_013961E8);
}

float FABLE_FASTCALL GFVYToAY(float value)
{
    CDisplayManager* display = GFGetSystemManager()->PDisplayManager.Data;
    return (float)(display->GetRenderTargetDimensions().Y * value / g_FableVirtualHeight_013961F0);
}

// lib_global_tools.cpp, Ego_r ?GFVCoordToACoord@@YIXABVC2DVector@@AAV1@@Z
// (parameters s_pos/d_pos), retail 009E22C0: the scalar conversions above
// expanded per component. The C2DCoordI overload (009E2330) is below.
void FABLE_FASTCALL GFVCoordToACoord(const C2DVector& s_pos, C2DVector& d_pos)
{
    d_pos.X = GFVXToAX(s_pos.X);
    d_pos.Y = GFVYToAY(s_pos.Y);
}

// lib_global_tools.cpp, Ego_r ?GFVBoxToABox@@YIXPBVC2DBoxF@@PAV1@@Z (s_box,
// d_box), retail 009E1E70: size first, then the top-left corner, both through
// the scalar conversions above.
void FABLE_FASTCALL GFVBoxToABox(const C2DBoxF* s_box, C2DBoxF* d_box)
{
    float width = GFVXToAX(s_box->right - s_box->left);
    float height = GFVYToAY(s_box->bottom - s_box->top);
    d_box->left = GFVXToAX(s_box->left);
    d_box->top = GFVYToAY(s_box->top);
    d_box->right = d_box->left + width;
    d_box->bottom = d_box->top + height;
}

// Retail 009E1F50 (Ego_r ?GFVBoxToABox@@YIXABVC2DBoxI@@AAV1@@Z): the integer
// box, corner by corner: the dimension goes to float and multiplies the
// integer corner directly (retail fimul), then divides by the virtual size.
static __forceinline long boxActualX(long value)
{
    CDisplayManager* display = GFGetSystemManager()->PDisplayManager.Data;
    return (long)((float)display->GetRenderTargetDimensions().X * value / g_FableVirtualWidth_013961E8);
}
static __forceinline long boxActualY(long value)
{
    CDisplayManager* display = GFGetSystemManager()->PDisplayManager.Data;
    return (long)((float)display->GetRenderTargetDimensions().Y * value / g_FableVirtualHeight_013961F0);
}
void FABLE_FASTCALL GFVBoxToABox(const C2DBoxI& s_box, C2DBoxI& d_box)
{
    d_box.left = boxActualX(s_box.left);
    d_box.top = boxActualY(s_box.top);
    d_box.right = boxActualX(s_box.right);
    d_box.bottom = boxActualY(s_box.bottom);
}

// Retail 009E2330 (Ego_r ?GFVCoordToACoord@@YIXABVC2DCoordI@@AAV1@@Z).
void FABLE_FASTCALL GFVCoordToACoord(const C2DCoordI& s_pos, C2DCoordI& d_pos)
{
    d_pos.X = (long)GFVXToAX((float)s_pos.X);
    d_pos.Y = (long)GFVYToAY((float)s_pos.Y);
}
