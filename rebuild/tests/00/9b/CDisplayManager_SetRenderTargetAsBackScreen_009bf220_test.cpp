#include <stdio.h>

#include "rebuild_abi.h"

struct C2DExtentsI
{
    fable_i32 width;
    fable_i32 height;
};

struct C2DBoxI
{
    fable_i32 left;
    fable_i32 top;
    fable_i32 right;
    fable_i32 bottom;
};

struct CSurface
{
    void* vtable00;
    void* resource04;
    fable_u32 resourceState08;
    fable_u32 unknown0C;

    CSurface& operator=(const CSurface& source);
    C2DExtentsI GetDimensions() const;

    __forceinline void* GetSurface() const
    {
        return resource04;
    }
};

struct FableDisplayDevice
{
    virtual void __stdcall Slot00() = 0;
    virtual void __stdcall Slot01() = 0;
    virtual void __stdcall Slot02() = 0;
    virtual void __stdcall Slot03() = 0;
    virtual void __stdcall Slot04() = 0;
    virtual void __stdcall Slot05() = 0;
    virtual void __stdcall Slot06() = 0;
    virtual void __stdcall Slot07() = 0;
    virtual void __stdcall Slot08() = 0;
    virtual void __stdcall Slot09() = 0;
    virtual void __stdcall Slot0A() = 0;
    virtual void __stdcall Slot0B() = 0;
    virtual void __stdcall Slot0C() = 0;
    virtual void __stdcall Slot0D() = 0;
    virtual void __stdcall Slot0E() = 0;
    virtual void __stdcall Slot0F() = 0;
    virtual void __stdcall Slot10() = 0;
    virtual void __stdcall Slot11() = 0;
    virtual void __stdcall Slot12() = 0;
    virtual void __stdcall Slot13() = 0;
    virtual void __stdcall Slot14() = 0;
    virtual void __stdcall Slot15() = 0;
    virtual void __stdcall Slot16() = 0;
    virtual void __stdcall Slot17() = 0;
    virtual void __stdcall Slot18() = 0;
    virtual void __stdcall Slot19() = 0;
    virtual void __stdcall Slot1A() = 0;
    virtual void __stdcall Slot1B() = 0;
    virtual void __stdcall Slot1C() = 0;
    virtual void __stdcall Slot1D() = 0;
    virtual void __stdcall Slot1E() = 0;
    virtual void __stdcall Slot1F() = 0;
    virtual void __stdcall Slot20() = 0;
    virtual void __stdcall Slot21() = 0;
    virtual void __stdcall Slot22() = 0;
    virtual void __stdcall Slot23() = 0;
    virtual void __stdcall Slot24() = 0;
    virtual long __stdcall SetRenderTarget(
        fable_u32 index,
        void* surface) = 0;
    virtual void __stdcall Slot26() = 0;
    virtual long __stdcall SetDepthStencilSurface(
        void* surface) = 0;
};

struct CDisplayManagerBackScreenView
{
    fable_u8 unknown000[0x0C];
    CSurface renderColour0C;
    CSurface renderDepth1C;
    CSurface backColour2C;
    CSurface backDepth3C;
    fable_u8 unknown04C[0x0C];
    FableDisplayDevice* displayDevice58;
    fable_u8 unknown05C[0x138];
    C2DExtentsI renderTargetDimensions194;

    void InitVirtualPixels();
    void SetViewport(const C2DBoxI& viewport);
    bool SetRenderTargetAsBackScreen();
};

static C2DExtentsI g_dimensions = {640, 480};
static int g_virtualPixelCalls = 0;
static int g_viewportCalls = 0;
static C2DBoxI g_viewport = {};
static void* g_expectedColour = 0;
static void* g_expectedDepth = 0;
static long g_colourResult = 0;
static long g_depthResult = 0;

CSurface& CSurface::operator=(const CSurface& source)
{
    resource04 = source.resource04;
    resourceState08 = source.resourceState08;
    return *this;
}

C2DExtentsI CSurface::GetDimensions() const
{
    return g_dimensions;
}

void CDisplayManagerBackScreenView::InitVirtualPixels()
{
    ++g_virtualPixelCalls;
}

void CDisplayManagerBackScreenView::SetViewport(
    const C2DBoxI& viewport)
{
    ++g_viewportCalls;
    g_viewport = viewport;
}

static long __stdcall SetColourTarget(
    FableDisplayDevice*,
    fable_u32 index,
    void* surface)
{
    if (index != 0 || surface != g_expectedColour)
        return -1;
    return g_colourResult;
}

static long __stdcall SetDepthTarget(
    FableDisplayDevice*,
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
    vtable[0x25] =
        reinterpret_cast<void*>(SetColourTarget);
    vtable[0x27] =
        reinterpret_cast<void*>(SetDepthTarget);
    void* deviceStorage = vtable;
    FableDisplayDevice* device =
        reinterpret_cast<FableDisplayDevice*>(
            &deviceStorage);
    CDisplayManagerBackScreenView display = {};
    display.displayDevice58 = device;
    display.backColour2C.resource04 = &colourResource;
    display.backColour2C.resourceState08 = 11;
    display.backDepth3C.resource04 = &depthResource;
    display.backDepth3C.resourceState08 = 22;
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
        display.renderColour0C.resource04 != &colourResource ||
        display.renderDepth1C.resource04 != &depthResource)
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
