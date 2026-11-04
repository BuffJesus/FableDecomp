#include <stdio.h>
#include <string.h>

#include "fable_display_viewport.h"
#include "fable_display_geometry.h"

static C2DExtentsI g_dimensions(640, 480);
static int g_virtualPixelCalls = 0;
static int g_viewportCalls = 0;
static C2DBoxI g_viewport = {};
static void* g_expectedColour = 0;
static void* g_expectedDepth = 0;
static long g_colourResult = 0;
static long g_depthResult = 0;

CSurface& CSurface::operator=(const CSurface& source)
{
    PD3DSurface = source.PD3DSurface;
    AllocationSource = source.AllocationSource;
    return *this;
}

C2DExtentsI CSurface::GetDimensions() const
{
    return g_dimensions;
}

void CSurface::Uninitialise()
{
}

void CDisplayManager::InitVirtualPixels()
{
    ++g_virtualPixelCalls;
}

void CDisplayManager::SetViewport(
    const C2DBoxI& viewport)
{
    ++g_viewportCalls;
    g_viewport = viewport;
}

static long __stdcall SetColourTarget(
    IDirect3DDevice9*,
    fable_u32 index,
    void* surface)
{
    if (index != 0 || surface != g_expectedColour)
        return -1;
    return g_colourResult;
}

static long __stdcall SetDepthTarget(
    IDirect3DDevice9*,
    void* surface)
{
    if (surface != g_expectedDepth)
        return -1;
    return g_depthResult;
}

static void ResetObservations()
{
    g_virtualPixelCalls = 0;
    g_viewportCalls = 0;
    g_viewport.left = -1;
    g_viewport.top = -1;
    g_viewport.right = -1;
    g_viewport.bottom = -1;
}

int main()
{
    int colourResource;
    int depthResource;
    void* vtable[0x28] = {};
    vtable[0x25] = reinterpret_cast<void*>(SetColourTarget);
    vtable[0x27] = reinterpret_cast<void*>(SetDepthTarget);
    void* deviceStorage = vtable;
    IDirect3DDevice9* device =
        reinterpret_cast<IDirect3DDevice9*>(&deviceStorage);

    // CDisplayManager has no default constructor (retail builds it from
    // CSystemManager); the call only needs its storage.
    static fable_u8 displayStorage[sizeof(CDisplayManager)];
    memset(displayStorage, 0, sizeof(displayStorage));
    CDisplayManager& display =
        *reinterpret_cast<CDisplayManager*>(displayStorage);
    display.D3DDevice = device;
    display.BackBuffer.PD3DSurface =
        reinterpret_cast<IDirect3DSurface9*>(&colourResource);
    display.BackBuffer.AllocationSource = 11;
    display.DepthBuffer.PD3DSurface =
        reinterpret_cast<IDirect3DSurface9*>(&depthResource);
    display.DepthBuffer.AllocationSource = 22;
    g_expectedColour = &colourResource;
    g_expectedDepth = &depthResource;

    ResetObservations();
    g_colourResult = 0;
    g_depthResult = 0;
    if (!display.SetRenderTargetAsBackScreen() ||
        g_virtualPixelCalls != 1 ||
        g_viewportCalls != 1 ||
        g_viewport.left != 0 ||
        g_viewport.top != 0 ||
        g_viewport.right != 640 ||
        g_viewport.bottom != 480 ||
        display.RenderTargetDimensions().X != 640 ||
        display.RenderTargetDimensions().Y != 480 ||
        reinterpret_cast<void*>(display.RenderTargetSurface.PD3DSurface) != &colourResource ||
        display.RenderTargetSurface.AllocationSource != 11 ||
        reinterpret_cast<void*>(display.RenderTargetDepthBuffer.PD3DSurface) != &depthResource ||
        display.RenderTargetDepthBuffer.AllocationSource != 22)
    {
        printf("FABLETLC_DISPLAY_BACK_SCREEN_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    ResetObservations();
    g_colourResult = -1;
    if (display.SetRenderTargetAsBackScreen() ||
        g_virtualPixelCalls != 1 ||
        g_viewportCalls != 0)
    {
        printf("FABLETLC_DISPLAY_BACK_SCREEN_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    ResetObservations();
    g_colourResult = 0;
    g_depthResult = -1;
    if (display.SetRenderTargetAsBackScreen() ||
        g_virtualPixelCalls != 1 ||
        g_viewportCalls != 0)
    {
        printf("FABLETLC_DISPLAY_BACK_SCREEN_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    printf("FABLETLC_DISPLAY_BACK_SCREEN_BEHAVIOR PASS\n");
    return 0;
}
