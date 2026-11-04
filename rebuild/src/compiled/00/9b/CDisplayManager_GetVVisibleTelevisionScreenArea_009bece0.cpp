#include "fable_display_viewport.h"
#include "fable_virtual_pixels.h"

// Retail 009BECE0 uses the complete virtual canvas, independent of the
// display device's physical resolution and the current viewport.
C2DBoxF CDisplayManager::GetVVisibleTelevisionScreenArea() const
{
    return C2DBoxF(0.0f, 0.0f,
        static_cast<float>(g_FableVirtualWidth_013961E8),
        static_cast<float>(g_FableVirtualHeight_013961F0));
}
