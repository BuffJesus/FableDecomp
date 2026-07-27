#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct FablePixelShaderDevice
{
    void** vtable;
};

struct CCountedPixelShaderView
{
    virtual void Release(fable_i32 freeMemory) = 0;
    void* pixelShader04;
    fable_i32 referenceCount08;
};

struct CCountedPixelShaderHandle
{
    CCountedPixelShaderView* pointer;
};

struct CShaderRenderManagerDisablePixelShaderView
{
    fable_u8 unknown000[0x1D4];
    FablePixelShaderDevice* displayDevice1D4;
    fable_u8 unknown1D8[0x0C];
    CCountedPixelShaderHandle currentPixelShader1E4;
    fable_u8 unknown1E8[0x1A9];
    fable_u8 pixelShadersEnabled391;
    fable_u8 unknown392[0x46];
    fable_u32 shaderStateMask3D8;

    void DisablePixelShaders();
};

static fable_i32 g_DeviceCalls;
static void* g_BoundShader;
static fable_i32 g_ReleaseCalls;
static fable_i32 g_FreeMemory;

static long __stdcall ObserveSetPixelShader(
    FablePixelShaderDevice*,
    void* shader)
{
    ++g_DeviceCalls;
    g_BoundShader = shader;
    return 0;
}

struct TestPixelShader : CCountedPixelShaderView
{
    virtual void Release(fable_i32 freeMemory)
    {
        ++g_ReleaseCalls;
        g_FreeMemory = freeMemory;
    }
};

int main()
{
    void* vtable[0x6C] = {0};
    vtable[0x6B] =
        reinterpret_cast<void*>(&ObserveSetPixelShader);
    FablePixelShaderDevice device = {vtable};
    TestPixelShader shader;
    memset(
        reinterpret_cast<fable_u8*>(&shader) + sizeof(void*),
        0,
        sizeof(shader) - sizeof(void*));
    CShaderRenderManagerDisablePixelShaderView manager;
    memset(&manager, 0, sizeof(manager));
    manager.displayDevice1D4 = &device;
    manager.currentPixelShader1E4.pointer = &shader;
    manager.shaderStateMask3D8 = 0xFFFFFFFF;

    manager.DisablePixelShaders();
    if (g_DeviceCalls != 0 ||
        manager.currentPixelShader1E4.pointer != &shader ||
        manager.shaderStateMask3D8 != 0xFFFFFFFF)
    {
        printf(
            "FABLETLC_DISABLE_PIXEL_SHADERS_BEHAVIOR "
            "FAIL code=1\n");
        return 1;
    }

    manager.pixelShadersEnabled391 = 1;
    shader.referenceCount08 = 2;
    manager.DisablePixelShaders();
    if (g_DeviceCalls != 1 ||
        g_BoundShader != 0 ||
        manager.pixelShadersEnabled391 != 0 ||
        manager.shaderStateMask3D8 != 0xFFFFFFFC ||
        manager.currentPixelShader1E4.pointer != 0 ||
        shader.referenceCount08 != 1 ||
        g_ReleaseCalls != 0)
    {
        printf(
            "FABLETLC_DISABLE_PIXEL_SHADERS_BEHAVIOR "
            "FAIL code=2\n");
        return 2;
    }

    manager.pixelShadersEnabled391 = 1;
    manager.shaderStateMask3D8 = 3;
    manager.currentPixelShader1E4.pointer = &shader;
    shader.referenceCount08 = 1;
    manager.DisablePixelShaders();
    if (g_DeviceCalls != 2 ||
        manager.shaderStateMask3D8 != 0 ||
        manager.currentPixelShader1E4.pointer != 0 ||
        shader.referenceCount08 != 0 ||
        g_ReleaseCalls != 1 ||
        g_FreeMemory != 1)
    {
        printf(
            "FABLETLC_DISABLE_PIXEL_SHADERS_BEHAVIOR "
            "FAIL code=3\n");
        return 3;
    }

    printf(
        "FABLETLC_DISABLE_PIXEL_SHADERS_BEHAVIOR PASS\n");
    return 0;
}
