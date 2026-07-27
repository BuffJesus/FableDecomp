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

extern FablePixelFormatInfo
    g_FablePixelFormatInfoTable_0129BA40[];

fable_i32 FABLE_FASTCALL
FableFindFormatInfoIndexFromD3DFormat_009E3620(
    fable_i32 d3dFormat);

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

void CDisplayManagerClearRenderTargetView::ClearRenderTarget(
    const CRGBColour& colour,
    fable_u32 requestedFlags)
{
    fable_u32 deviceFlags = 0;
    if ((requestedFlags & 1) != 0)
        deviceFlags = 2;
    if ((requestedFlags & 2) != 0)
    {
        const fable_i32 formatIndex =
            FableFindFormatInfoIndexFromD3DFormat_009E3620(
                g_FablePixelFormatInfoTable_0129BA40[
                    depthFormatIndex1AC].d3dFormat00);
        if (g_FablePixelFormatInfoTable_0129BA40[
                formatIndex].supportsDepthStencil1C != 0)
        {
            deviceFlags |= 4;
        }
    }
    if ((requestedFlags & 4) != 0)
        deviceFlags |= 1;

    displayDevice58->vtable[0x2B](
        displayDevice58,
        0,
        0,
        deviceFlags,
        colour.value,
        1.0f,
        0);
}
