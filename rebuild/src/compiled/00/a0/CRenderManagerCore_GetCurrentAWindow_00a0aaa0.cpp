#include "fable_display_viewport.h"

const C2DBoxF& CRenderManagerCore::GetCurrentAWindow() const
{
    return DisplayManager->ViewportBoxFloat;
}
