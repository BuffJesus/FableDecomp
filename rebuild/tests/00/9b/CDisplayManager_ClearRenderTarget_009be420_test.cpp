#include <stdio.h>

#include "rebuild_abi.h"

struct CRGBColour
{
    fable_u32 value;
};

struct FablePixelFormatInfo
{
    fable_i32 d3dFormat00;
    fable_u8 unknown04[0x18];
    fable_i32 supportsDepthStencil1C;
    fable_u32 unknown20;
};

FablePixelFormatInfo
    g_FablePixelFormatInfoTable_0129BA40[2] = {};

static fable_i32 g_observedFormat = 0;

fable_i32 __fastcall
FableFindFormatInfoIndexFromD3DFormat_009E3620(
    fable_i32 d3dFormat)
{
    g_observedFormat = d3dFormat;
    return 1;
}

struct FableDisplayDevice;
typedef long (__stdcall *FableDisplayDeviceClearCall)(
    FableDisplayDevice* device,
    fable_u32 rectangleCount,
    void* rectangles,
    fable_u32 clearFlags,
    fable_u32 colour,
    float depth,
    fable_u32 stencil);

struct FableDisplayDevice
{
    FableDisplayDeviceClearCall* vtable;
};

struct CDisplayManagerClearRenderTargetView
{
    fable_u8 unknown000[0x58];
    FableDisplayDevice* displayDevice58;
    fable_u8 unknown05C[0x150];
    fable_u32 depthFormatIndex1AC;

    void ClearRenderTarget(
        const CRGBColour& colour,
        fable_u32 requestedFlags);
};

static int g_clearCalls = 0;
static fable_u32 g_observedFlags = 0;
static fable_u32 g_observedColour = 0;
static float g_observedDepth = 0.0f;

static long __stdcall ClearFixture(
    FableDisplayDevice*,
    fable_u32 rectangleCount,
    void* rectangles,
    fable_u32 clearFlags,
    fable_u32 colour,
    float depth,
    fable_u32 stencil)
{
    ++g_clearCalls;
    if (rectangleCount != 0 || rectangles != 0 || stencil != 0)
        return -1;
    g_observedFlags = clearFlags;
    g_observedColour = colour;
    g_observedDepth = depth;
    return 0;
}

static int VerifyFlags(
    CDisplayManagerClearRenderTargetView& display,
    const CRGBColour& colour,
    fable_u32 requested,
    fable_u32 expected)
{
    display.ClearRenderTarget(colour, requested);
    return g_observedFlags == expected ? 0 : 1;
}

int main()
{
    FableDisplayDeviceClearCall vtable[0x2C] = {};
    vtable[0x2B] = ClearFixture;
    FableDisplayDevice device = {vtable};
    CDisplayManagerClearRenderTargetView display = {};
    display.displayDevice58 = &device;
    display.depthFormatIndex1AC = 0;

    g_FablePixelFormatInfoTable_0129BA40[0].d3dFormat00 = 75;
    g_FablePixelFormatInfoTable_0129BA40[1].supportsDepthStencil1C = 1;
    const CRGBColour colour = {0xA1B2C3D4};

    if (VerifyFlags(display, colour, 1, 2) != 0 ||
        VerifyFlags(display, colour, 2, 4) != 0 ||
        VerifyFlags(display, colour, 4, 1) != 0 ||
        VerifyFlags(display, colour, 7, 7) != 0)
    {
        printf("FABLETLC_DISPLAY_CLEAR_TARGET_BEHAVIOR FAIL code=1\n");
        return 1;
    }
    if (g_clearCalls != 4 ||
        g_observedFormat != 75 ||
        g_observedColour != colour.value ||
        g_observedDepth != 1.0f)
    {
        printf("FABLETLC_DISPLAY_CLEAR_TARGET_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_DISPLAY_CLEAR_TARGET_BEHAVIOR PASS\n");
    return 0;
}
