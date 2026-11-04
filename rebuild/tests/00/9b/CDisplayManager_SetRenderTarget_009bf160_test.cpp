#include <stdio.h>
#include <string.h>

#include "fable_display_viewport.h"
#include "engine/CRenderTarget.h"

static C2DExtentsI g_dimensions(800, 600);
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

    // CDisplayManager / CRenderTarget have no default constructors (retail
    // builds them from CSystemManager / two surfaces); the call only needs
    // their storage.
    static fable_u8 displayStorage[sizeof(CDisplayManager)];
    static fable_u8 targetStorage[sizeof(CRenderTarget)];
    memset(displayStorage, 0, sizeof(displayStorage));
    memset(targetStorage, 0, sizeof(targetStorage));
    CDisplayManager& display =
        *reinterpret_cast<CDisplayManager*>(displayStorage);
    CRenderTarget& target =
        *reinterpret_cast<CRenderTarget*>(targetStorage);
    display.D3DDevice = device;
    target.RenderToSurface.PD3DSurface =
        reinterpret_cast<IDirect3DSurface9*>(&colourResource);
    target.RenderToSurface.AllocationSource = 31;
    target.ZBufferSurface.PD3DSurface =
        reinterpret_cast<IDirect3DSurface9*>(&depthResource);
    target.ZBufferSurface.AllocationSource = 47;
    g_expectedColour = &colourResource;
    g_expectedDepth = &depthResource;

    ResetObservations();
    g_colourResult = 0;
    g_depthResult = 0;
    if (!display.SetRenderTarget(target) ||
        g_virtualPixelCalls != 1 ||
        g_viewportCalls != 1 ||
        g_viewport.left != 0 ||
        g_viewport.top != 0 ||
        g_viewport.right != 800 ||
        g_viewport.bottom != 600 ||
        display.RenderTargetDimensions().X != 800 ||
        display.RenderTargetDimensions().Y != 600 ||
        reinterpret_cast<void*>(display.RenderTargetSurface.PD3DSurface) != &colourResource ||
        display.RenderTargetSurface.AllocationSource != 31 ||
        reinterpret_cast<void*>(display.RenderTargetDepthBuffer.PD3DSurface) != &depthResource ||
        display.RenderTargetDepthBuffer.AllocationSource != 47)
    {
        printf("FABLETLC_DISPLAY_SET_RENDER_TARGET_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    ResetObservations();
    g_colourResult = -1;
    if (display.SetRenderTarget(target) ||
        g_virtualPixelCalls != 1 ||
        g_viewportCalls != 0)
    {
        printf("FABLETLC_DISPLAY_SET_RENDER_TARGET_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    ResetObservations();
    g_colourResult = 0;
    g_depthResult = -1;
    if (display.SetRenderTarget(target) ||
        g_virtualPixelCalls != 1 ||
        g_viewportCalls != 0)
    {
        printf("FABLETLC_DISPLAY_SET_RENDER_TARGET_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    printf("FABLETLC_DISPLAY_SET_RENDER_TARGET_BEHAVIOR PASS\n");
    return 0;
}
