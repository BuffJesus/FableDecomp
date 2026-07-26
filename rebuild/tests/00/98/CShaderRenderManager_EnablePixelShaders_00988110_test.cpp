#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct CDisplayManagerPixelShaderView
{
    fable_u32 GetPixelShaderVersion() const;
};

struct CDisplayEnginePixelShaderView
{
    fable_u8 unknown0000[0x3A3C];
    CDisplayManagerPixelShaderView* displayManager3A3C;
};

struct CShaderRenderManagerPixelShaderView
{
    fable_u8 unknown000[0x1D0];
    CDisplayEnginePixelShaderView* displayEngine1D0;
    fable_u8 unknown1D4[0x1BD];
    fable_u8 pixelShadersEnabled391;

    void EnablePixelShaders();
};

static fable_u32 g_PixelShaderVersion = 0;

fable_u32
CDisplayManagerPixelShaderView::GetPixelShaderVersion() const
{
    return g_PixelShaderVersion;
}

int main()
{
    CDisplayManagerPixelShaderView displayManager;
    CDisplayEnginePixelShaderView displayEngine;
    CShaderRenderManagerPixelShaderView manager;
    memset(&displayEngine, 0, sizeof(displayEngine));
    memset(&manager, 0, sizeof(manager));
    displayEngine.displayManager3A3C = &displayManager;
    manager.displayEngine1D0 = &displayEngine;

    g_PixelShaderVersion = 999;
    manager.EnablePixelShaders();
    if (manager.pixelShadersEnabled391 != 0)
    {
        return 1;
    }

    g_PixelShaderVersion = 1000;
    manager.EnablePixelShaders();
    if (manager.pixelShadersEnabled391 != 1)
    {
        return 1;
    }

    printf("FABLETLC_ENABLE_PIXEL_SHADERS_BEHAVIOR PASS\n");
    return 0;
}
