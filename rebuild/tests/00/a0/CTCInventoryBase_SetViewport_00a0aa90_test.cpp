#include <stdio.h>

struct C2DBoxI
{
    int left;
    int top;
    int right;
    int bottom;
};

struct CDisplayManager
{
    void SetViewport(const C2DBoxI& viewport);
};

struct CTCInventoryBaseViewportView
{
    unsigned char unknown0000[0x3A3C];
    CDisplayManager* displayManager3A3C;

    void SetViewport_00A0AA90(const C2DBoxI& viewport);
};

static CDisplayManager* g_observedManager = 0;
static const C2DBoxI* g_observedViewport = 0;

void CDisplayManager::SetViewport(const C2DBoxI& viewport)
{
    g_observedManager = this;
    g_observedViewport = &viewport;
}

int main()
{
    CDisplayManager manager;
    CTCInventoryBaseViewportView inventory = {};
    C2DBoxI viewport = {10, 20, 630, 460};
    inventory.displayManager3A3C = &manager;

    inventory.SetViewport_00A0AA90(viewport);
    if (g_observedManager != &manager)
    {
        printf("FABLETLC_INVENTORY_VIEWPORT_BEHAVIOR FAIL code=1\n");
        return 1;
    }
    if (g_observedViewport != &viewport)
    {
        printf("FABLETLC_INVENTORY_VIEWPORT_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_INVENTORY_VIEWPORT_BEHAVIOR PASS\n");
    return 0;
}
