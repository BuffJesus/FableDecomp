#include <stdio.h>

#include "fable_render_window.h"

static CDisplayManager* g_ObservedManager = 0;
static const C2DBoxF* g_ObservedViewport = 0;

void CDisplayManager::SetViewport(
    const C2DBoxF& viewport)
{
    g_ObservedManager = this;
    g_ObservedViewport = &viewport;
}

int main()
{
    CDisplayManager displayManager;
    CRenderManagerCoreWindowView rendererCore = {};
    const C2DBoxF viewport = {1.0f, 2.0f, 639.0f, 479.0f};
    rendererCore.displayManager3A3C = &displayManager;

    rendererCore.SetAWindow(viewport);
    if (
        g_ObservedManager != &displayManager ||
        g_ObservedViewport != &viewport)
    {
        return 1;
    }

    printf("FABLETLC_RENDER_MANAGER_SET_A_WINDOW_BEHAVIOR PASS\n");
    return 0;
}
