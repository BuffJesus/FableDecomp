#include "fable_display_viewport.h"

// FableWin's method is non-const; the returned C2DExtentsI is const.
const C2DExtentsI CDisplayManager::GetRenderTargetDimensions()
{
    return C2DExtentsI(RenderTargetDimensionsStorage_[0],RenderTargetDimensionsStorage_[1]);
}
