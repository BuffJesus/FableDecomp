#include "fable_display_viewport.h"
#include "fable_virtual_pixels.h"
void CRenderManagerCore::SetVWindow(const C2DBoxF& window)
{
    C2DBoxF actual;
    GFVBoxToABox(&window,&actual);
    DisplayManager->SetViewport(actual);
}
