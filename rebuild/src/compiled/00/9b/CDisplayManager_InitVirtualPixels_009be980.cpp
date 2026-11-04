// Retail 009BE980; FableWin private CDisplayManager::InitVirtualPixels.
#include "fable_display_viewport.h"
#include "fable_virtual_pixels.h"
#include <math.h>
void CDisplayManager::InitVirtualPixels()
{
    VirtualCoordsResolutionIndependant=true;
    FableVirtualPixelScaleX_013ca800=65536.0/RenderTargetDimensionsStorage_[0];
    FableVirtualPixelScaleY_013ca808=65536.0/RenderTargetDimensionsStorage_[1];
    FableVirtualPixelScaleX_013ca800=floor(FableVirtualPixelScaleX_013ca800);
    FableVirtualPixelScaleY_013ca808=floor(FableVirtualPixelScaleY_013ca808);
    g_FableVirtualWidth_013961E8=RenderTargetDimensionsStorage_[0]*FableVirtualPixelScaleX_013ca800;
    g_FableVirtualHeight_013961F0=RenderTargetDimensionsStorage_[1]*FableVirtualPixelScaleY_013ca808;
}
