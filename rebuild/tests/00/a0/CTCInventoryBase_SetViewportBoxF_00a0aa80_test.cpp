#include <stdio.h>

struct C2DBoxF
{
    float left;
    float top;
    float right;
    float bottom;
};

struct CDisplayManagerViewportBoxFView
{
    void SetViewport(const C2DBoxF& viewport);
};

struct CTCInventoryBaseViewportBoxFView
{
    unsigned char unknown0000[0x3A3C];
    CDisplayManagerViewportBoxFView* displayManager3A3C;

    void SetViewport(const C2DBoxF& viewport);
};

static CDisplayManagerViewportBoxFView* g_ObservedManager = 0;
static const C2DBoxF* g_ObservedViewport = 0;

void CDisplayManagerViewportBoxFView::SetViewport(
    const C2DBoxF& viewport)
{
    g_ObservedManager = this;
    g_ObservedViewport = &viewport;
}

int main()
{
    CDisplayManagerViewportBoxFView displayManager;
    CTCInventoryBaseViewportBoxFView inventory = {};
    const C2DBoxF viewport = {1.0f, 2.0f, 639.0f, 479.0f};
    inventory.displayManager3A3C = &displayManager;

    inventory.SetViewport(viewport);
    if (
        g_ObservedManager != &displayManager ||
        g_ObservedViewport != &viewport)
    {
        return 1;
    }

    printf("FABLETLC_INVENTORY_BOXF_VIEWPORT_BEHAVIOR PASS\n");
    return 0;
}
