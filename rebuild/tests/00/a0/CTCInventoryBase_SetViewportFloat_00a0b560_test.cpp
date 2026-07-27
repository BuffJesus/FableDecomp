#include <stdio.h>

struct C2DBoxF
{
    float left;
    float top;
    float right;
    float bottom;
};

struct C2DBoxI
{
    int left;
    int top;
    int right;
    int bottom;
};

void __fastcall
GFVBoxToABox(
    const C2DBoxF* source,
    C2DBoxI* destination)
{
    destination->left = (int)source->left;
    destination->top = (int)source->top;
    destination->right = (int)source->right;
    destination->bottom = (int)source->bottom;
}

struct CDisplayManager
{
    void SetViewport(const C2DBoxI& viewport);
};

struct CTCInventoryBaseViewportFloatView
{
    unsigned char unknown0000[0x3A3C];
    CDisplayManager* displayManager3A3C;

    void SetViewportFloat_00A0B560(const C2DBoxF& viewport);
};

static CDisplayManager* g_observedManager = 0;
static C2DBoxI g_observedViewport = {};

void CDisplayManager::SetViewport(const C2DBoxI& viewport)
{
    g_observedManager = this;
    g_observedViewport = viewport;
}

int main()
{
    CDisplayManager manager;
    CTCInventoryBaseViewportFloatView inventory = {};
    C2DBoxF viewport = {10.5f, 20.5f, 630.5f, 460.5f};
    inventory.displayManager3A3C = &manager;

    inventory.SetViewportFloat_00A0B560(viewport);
    if (g_observedManager != &manager)
    {
        printf("FABLETLC_INVENTORY_FLOAT_VIEWPORT_BEHAVIOR FAIL code=1\n");
        return 1;
    }
    if (g_observedViewport.left != 10 ||
        g_observedViewport.top != 20 ||
        g_observedViewport.right != 630 ||
        g_observedViewport.bottom != 460)
    {
        printf("FABLETLC_INVENTORY_FLOAT_VIEWPORT_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_INVENTORY_FLOAT_VIEWPORT_BEHAVIOR PASS\n");
    return 0;
}
