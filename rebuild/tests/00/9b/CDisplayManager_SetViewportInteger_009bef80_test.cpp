#include <stdio.h>
#include <string.h>

#include "fable_display_viewport.h"
#include "fable_shader_render_manager.h"

struct FableD3DViewport
{
    fable_u32 x;
    fable_u32 y;
    fable_u32 width;
    fable_u32 height;
    float minZ;
    float maxZ;
};

CShaderRenderManager g_CShaderRenderManager_013BC470;

static fable_u32 g_shaderNotifications = 0;
static fable_u32 g_legacyNotifications = 0;
static fable_u32 g_deviceCalls = 0;
static fable_i32 g_deviceResult = 0;
static FableD3DViewport g_observedViewport = {};

void CShaderRenderManager::OnPostViewportChanged()
{
    ++g_shaderNotifications;
}

extern "C" void FableViewportE2_00A0AAC0()
{
    ++g_legacyNotifications;
}

extern "C" fable_i32 __stdcall TestSetViewport(
    void*,
    const FableD3DViewport* viewport)
{
    ++g_deviceCalls;
    g_observedViewport = *viewport;
    return g_deviceResult;
}

static fable_i32 ReadI32(const fable_u8* bytes, fable_u32 offset)
{
    return *reinterpret_cast<const fable_i32*>(bytes + offset);
}

static float ReadFloat(const fable_u8* bytes, fable_u32 offset)
{
    return *reinterpret_cast<const float*>(bytes + offset);
}

static void WriteI32(fable_u8* bytes, fable_u32 offset, fable_i32 value)
{
    *reinterpret_cast<fable_i32*>(bytes + offset) = value;
}

static int CheckViewport(
    fable_u32 x,
    fable_u32 y,
    fable_u32 width,
    fable_u32 height)
{
    return g_observedViewport.x == x &&
           g_observedViewport.y == y &&
           g_observedViewport.width == width &&
           g_observedViewport.height == height &&
           g_observedViewport.minZ == 0.0f &&
           g_observedViewport.maxZ == 1.0f;
}

int main()
{
    void* deviceVtable[48] = {};
    void* deviceObject[1] = {deviceVtable};
    deviceVtable[47] = reinterpret_cast<void*>(&TestSetViewport);

    fable_u8 displayBytes[0x214];
    memset(displayBytes, 0, sizeof(displayBytes));
    *reinterpret_cast<void***>(displayBytes + 0x58) = deviceObject;
    WriteI32(displayBytes, 0x194, 16);
    WriteI32(displayBytes, 0x198, 10);
    WriteI32(displayBytes, 0x1F4, -1);
    WriteI32(displayBytes, 0x1F8, -1);
    WriteI32(displayBytes, 0x1FC, -1);
    WriteI32(displayBytes, 0x200, -1);

    CDisplayManager* display =
        reinterpret_cast<CDisplayManager*>(displayBytes);
    C2DBoxI clipped = {-4, 2, 20, 12};
    display->SetIntegerViewportEndpoint(clipped);
    if (g_deviceCalls != 1 ||
        g_shaderNotifications != 1 ||
        g_legacyNotifications != 0 ||
        !CheckViewport(0, 2, 16, 8) ||
        displayBytes[0x1D8] != 0 ||
        ReadI32(displayBytes, 0x1F4) != 0 ||
        ReadI32(displayBytes, 0x1F8) != 2 ||
        ReadI32(displayBytes, 0x1FC) != 16 ||
        ReadI32(displayBytes, 0x200) != 10 ||
        ReadFloat(displayBytes, 0x204) != 0.0f ||
        ReadFloat(displayBytes, 0x208) != 2.0f ||
        ReadFloat(displayBytes, 0x20C) != 16.0f ||
        ReadFloat(displayBytes, 0x210) != 10.0f)
    {
        printf("FABLETLC_DISPLAY_INTEGER_VIEWPORT_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    // A repeated clamped box returns before hardware or notifications.
    display->SetIntegerViewportEndpoint(clipped);
    if (g_deviceCalls != 1 ||
        g_shaderNotifications != 1 ||
        g_legacyNotifications != 0)
    {
        printf("FABLETLC_DISPLAY_INTEGER_VIEWPORT_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    // A zero-area box is expanded to one pixel and marks the special flag.
    C2DBoxI empty = {-3, -4, -2, -3};
    display->SetIntegerViewportEndpoint(empty);
    if (g_deviceCalls != 2 ||
        g_shaderNotifications != 2 ||
        !CheckViewport(0, 0, 1, 1) ||
        displayBytes[0x1D8] != 1)
    {
        printf("FABLETLC_DISPLAY_INTEGER_VIEWPORT_BEHAVIOR FAIL code=3\n");
        return 3;
    }

    // Hardware failure leaves the successful-input cache untouched, but the
    // D3D viewport and post-change notifications still run.
    displayBytes[0x08] = 1;
    g_deviceResult = -1;
    C2DBoxI failed = {3, 4, 6, 8};
    display->SetIntegerViewportEndpoint(failed);
    if (g_deviceCalls != 3 ||
        g_shaderNotifications != 3 ||
        g_legacyNotifications != 1 ||
        !CheckViewport(3, 4, 4, 5) ||
        ReadI32(displayBytes, 0x1F4) != 0 ||
        ReadI32(displayBytes, 0x1F8) != 0 ||
        ReadI32(displayBytes, 0x1FC) != 1 ||
        ReadI32(displayBytes, 0x200) != 1)
    {
        printf(
            "FABLETLC_DISPLAY_INTEGER_VIEWPORT_BEHAVIOR FAIL code=4 "
            "calls=%u shader=%u legacy=%u viewport=%u,%u,%u,%u "
            "cache=%d,%d,%d,%d\n",
            g_deviceCalls,
            g_shaderNotifications,
            g_legacyNotifications,
            g_observedViewport.x,
            g_observedViewport.y,
            g_observedViewport.width,
            g_observedViewport.height,
            ReadI32(displayBytes, 0x1F4),
            ReadI32(displayBytes, 0x1F8),
            ReadI32(displayBytes, 0x1FC),
            ReadI32(displayBytes, 0x200));
        return 4;
    }

    printf("FABLETLC_DISPLAY_INTEGER_VIEWPORT_BEHAVIOR PASS\n");
    return 0;
}
