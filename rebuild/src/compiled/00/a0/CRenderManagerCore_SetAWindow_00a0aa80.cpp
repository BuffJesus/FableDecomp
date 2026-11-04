#include "fable_display_viewport.h"

void CRenderManagerCore::SetAWindow(const C2DBoxF& window)
{
    DisplayManager->SetViewport(window);
}

void CRenderManagerCore::SetAWindow(const C2DBoxI& window)
{
    DisplayManager->SetIntegerViewportEndpoint(window);
}
