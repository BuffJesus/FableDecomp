// lib_display_manager.cpp, retail 0x009BEA10 (Ego_r
// ?SetVirtualCoordsAsResolutionIndependant@CDisplayManager@@QAEX_N@Z): either
// fit the 65536-unit virtual space to the render target (InitVirtualPixels,
// 0x009BE980, out of line), or fix the per-pixel scales at the 800x600 values
// and derive the virtual size from the render target.
#include <math.h>
#include "fable_display_viewport.h"
#include "fable_virtual_pixels.h"

void CDisplayManager::SetVirtualCoordsAsResolutionIndependant(bool independant)
{
    VirtualCoordsResolutionIndependant = independant;
    if (independant)
    {
        InitVirtualPixels();
        return;
    }
    FableVirtualPixelScaleX_013ca800 = 65536.0 / 800;
    FableVirtualPixelScaleY_013ca808 = 65536.0 / 600;
    FableVirtualPixelScaleX_013ca800 = floor(FableVirtualPixelScaleX_013ca800);
    FableVirtualPixelScaleY_013ca808 = floor(FableVirtualPixelScaleY_013ca808);
    g_FableVirtualWidth_013961E8 = RenderTargetDimensionsStorage_[0] * FableVirtualPixelScaleX_013ca800;
    g_FableVirtualHeight_013961F0 = RenderTargetDimensionsStorage_[1] * FableVirtualPixelScaleY_013ca808;
}
