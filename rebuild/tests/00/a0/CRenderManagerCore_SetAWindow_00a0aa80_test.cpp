#include <stdio.h>

struct C2DBoxF
{
    float left;
    float top;
    float right;
    float bottom;
};

struct CDisplayManagerWindowView
{
    void SetViewport(const C2DBoxF& viewport);
};

struct CRenderManagerCoreWindowView
{
    unsigned char unknown0000[0x3A3C];
    CDisplayManagerWindowView* displayManager3A3C;

    void SetAWindow(const C2DBoxF& window);
};

static CDisplayManagerWindowView* g_ObservedManager = 0;
static const C2DBoxF* g_ObservedViewport = 0;

void CDisplayManagerWindowView::SetViewport(
    const C2DBoxF& viewport)
{
    g_ObservedManager = this;
    g_ObservedViewport = &viewport;
}

int main()
{
    CDisplayManagerWindowView displayManager;
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
