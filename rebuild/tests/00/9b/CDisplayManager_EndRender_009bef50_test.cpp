#include <stdio.h>

struct CRenderManagerCore
{
    void EndRender();
};

struct FableDisplayDevice;
typedef long (__stdcall *FableDisplayDeviceCall)(FableDisplayDevice* device);

struct FableDisplayDevice
{
    FableDisplayDeviceCall* vtable;
};

struct CDisplayManagerEndRenderView
{
    unsigned char unknown000[0x08];
    CRenderManagerCore* renderManagerCore08;
    unsigned char unknown00C[0x4C];
    FableDisplayDevice* displayDevice58;
    unsigned char unknown05C[0x1B8];
    bool renderActive214;
    bool renderPending215;

    void EndRender();
};

static int g_coreEndCalls = 0;

void CRenderManagerCore::EndRender()
{
    ++g_coreEndCalls;
}

static long __stdcall EndSucceeded(FableDisplayDevice*)
{
    return 0;
}

static long __stdcall EndFailed(FableDisplayDevice*)
{
    return -1;
}

int main()
{
    FableDisplayDeviceCall vtable[0x2B] = {};
    FableDisplayDevice device = {vtable};
    CRenderManagerCore core;
    CDisplayManagerEndRenderView display = {};
    display.renderManagerCore08 = &core;
    display.displayDevice58 = &device;

    display.renderActive214 = true;
    display.renderPending215 = true;
    vtable[0x2A] = EndSucceeded;
    display.EndRender();
    if (display.renderActive214 ||
        display.renderPending215 ||
        g_coreEndCalls != 1)
    {
        printf("FABLETLC_DISPLAY_END_RENDER_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    display.renderActive214 = true;
    display.renderPending215 = true;
    vtable[0x2A] = EndFailed;
    display.EndRender();
    if (!display.renderActive214 ||
        !display.renderPending215 ||
        g_coreEndCalls != 2)
    {
        printf("FABLETLC_DISPLAY_END_RENDER_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_DISPLAY_END_RENDER_BEHAVIOR PASS\n");
    return 0;
}
