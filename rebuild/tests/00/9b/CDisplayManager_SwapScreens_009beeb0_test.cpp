#include <stdio.h>

#include "rebuild_abi.h"

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
    virtual long __stdcall Present(
        void* sourceRectangle,
        void* destinationRectangle,
        void* destinationWindowOverride,
        void* dirtyRegion) = 0;
};

struct CVsyncCallbackTimer
{
    void OnPostSwapScreens();
};

struct CDisplayManagerSwapScreensView
{
    fable_u8 unknown000[0x58];
    FableDisplayDevice* displayDevice58;
    fable_u8 unknown05C[0x140];
    fable_i32 backBufferCount19C;
    fable_u8 unknown1A0[0x78];
    fable_i32 currentBackBuffer218;
    fable_u8 unknown21C[0x39];
    fable_u8 deviceLost255;
    fable_u8 unknown256[0x06];
    fable_i32 presentCount25C;
    fable_u8 unknown260[0x34];
    CVsyncCallbackTimer* vsyncCallbackTimer294;

    bool SwapScreens();
};

static long g_presentResult = 0;
static int g_presentCalls = 0;
static int g_callbackCalls = 0;

static long __stdcall PresentFixture(
    FableDisplayDevice*,
    void* sourceRectangle,
    void* destinationRectangle,
    void* destinationWindowOverride,
    void* dirtyRegion)
{
    ++g_presentCalls;
    if (sourceRectangle != 0 ||
        destinationRectangle != 0 ||
        destinationWindowOverride != 0 ||
        dirtyRegion != 0)
    {
        return -2;
    }
    return g_presentResult;
}

void CVsyncCallbackTimer::OnPostSwapScreens()
{
    ++g_callbackCalls;
}

static void ResetObservations()
{
    g_presentCalls = 0;
    g_callbackCalls = 0;
}

int main()
{
    void* vtable[0x12] = {};
    vtable[0x11] = reinterpret_cast<void*>(PresentFixture);
    void* deviceStorage = vtable;
    FableDisplayDevice* device =
        reinterpret_cast<FableDisplayDevice*>(&deviceStorage);
    CVsyncCallbackTimer timer;
    CDisplayManagerSwapScreensView display = {};
    display.displayDevice58 = device;
    display.vsyncCallbackTimer294 = &timer;
    display.backBufferCount19C = 2;
    display.currentBackBuffer218 = 1;

    ResetObservations();
    g_presentResult = 0;
    if (!display.SwapScreens() ||
        g_presentCalls != 1 ||
        g_callbackCalls != 1 ||
        display.presentCount25C != 1 ||
        display.currentBackBuffer218 != 2 ||
        display.deviceLost255 != 0)
    {
        printf("FABLETLC_DISPLAY_SWAP_SCREENS_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    ResetObservations();
    display.currentBackBuffer218 = 2;
    if (!display.SwapScreens() ||
        display.currentBackBuffer218 != 0 ||
        display.presentCount25C != 2 ||
        g_callbackCalls != 1)
    {
        printf("FABLETLC_DISPLAY_SWAP_SCREENS_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    ResetObservations();
    g_presentResult = static_cast<long>(0x88760868);
    display.currentBackBuffer218 = 1;
    if (display.SwapScreens() ||
        display.deviceLost255 != 1 ||
        display.currentBackBuffer218 != 1 ||
        display.presentCount25C != 3 ||
        g_callbackCalls != 0)
    {
        printf("FABLETLC_DISPLAY_SWAP_SCREENS_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    ResetObservations();
    g_presentResult = -1;
    display.deviceLost255 = 0;
    if (display.SwapScreens() ||
        display.deviceLost255 != 0 ||
        display.currentBackBuffer218 != 1 ||
        display.presentCount25C != 4 ||
        g_callbackCalls != 0)
    {
        printf("FABLETLC_DISPLAY_SWAP_SCREENS_BEHAVIOR FAIL code=4\n");
        return 4;
    }

    printf("FABLETLC_DISPLAY_SWAP_SCREENS_BEHAVIOR PASS\n");
    return 0;
}
