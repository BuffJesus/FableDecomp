#include <stdio.h>

struct CRenderManagerCore
{
    void BeginRender();
};

struct FableDisplayDevice;
typedef long (__stdcall *FableDisplayDeviceCall)(FableDisplayDevice* device);

struct FableDisplayDevice
{
    FableDisplayDeviceCall* vtable;
};

struct CDisplayManagerBeginRenderView
{
    unsigned char unknown000[0x08];
    CRenderManagerCore* renderManagerCore08;
    unsigned char unknown00C[0x4C];
    FableDisplayDevice* displayDevice58;
    unsigned char unknown05C[0x1B8];
    bool renderActive214;

    bool BeginRender();
};

static int g_coreBeginCalls = 0;

void CRenderManagerCore::BeginRender()
{
    ++g_coreBeginCalls;
}

static long __stdcall BeginSucceeded(FableDisplayDevice*)
{
    return 0;
}

static long __stdcall BeginFailed(FableDisplayDevice*)
{
    return -1;
}

int main()
{
    FableDisplayDeviceCall vtable[0x2A] = {};
    FableDisplayDevice device = {vtable};
    CRenderManagerCore core;
    CDisplayManagerBeginRenderView display = {};
    display.renderManagerCore08 = &core;
    display.displayDevice58 = &device;

    vtable[0x29] = BeginSucceeded;
    if (!display.BeginRender() ||
        !display.renderActive214 ||
        g_coreBeginCalls != 1)
    {
        printf("FABLETLC_DISPLAY_BEGIN_RENDER_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    display.renderActive214 = false;
    vtable[0x29] = BeginFailed;
    if (display.BeginRender() ||
        display.renderActive214 ||
        g_coreBeginCalls != 1)
    {
        printf("FABLETLC_DISPLAY_BEGIN_RENDER_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_DISPLAY_BEGIN_RENDER_BEHAVIOR PASS\n");
    return 0;
}
