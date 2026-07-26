#include "fable_render_window.h"

void CRenderManagerCoreWindowView::SetAWindow(
    const C2DBoxF& window)
{
    displayManager3A3C->SetViewport(window);
}
