#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct FablePixelShaderDevice;
typedef long (__stdcall *FableSetPixelShaderCall)(
    FablePixelShaderDevice* device,
    void* pixelShader);

struct FablePixelShaderDevice
{
    void** vtable;
};

struct CCountedPixelShaderView
{
    void* unknown00;
    void* pixelShader04;
};

struct CShaderRenderManagerUpdatePixelShaderView
{
    fable_u8 unknown000[0x1D4];
    FablePixelShaderDevice* displayDevice1D4;
    fable_u8 unknown1D8[0x0C];
    CCountedPixelShaderView* currentPixelShader1E4;
    fable_u8 unknown1E8[0x1F0];
    fable_u32 shaderStateMask3D8;

    void UpdatePixelShader();
};

static int g_Calls = 0;
static void* g_Shader = 0;

static long __stdcall SetPixelShaderCall(
    FablePixelShaderDevice*,
    void* shader)
{
    ++g_Calls;
    g_Shader = shader;
    return 0;
}

int main()
{
    void* vtable[0x6C] = {0};
    vtable[0x6B] = reinterpret_cast<void*>(&SetPixelShaderCall);
    FablePixelShaderDevice device = {vtable};
    CCountedPixelShaderView shader = {0, reinterpret_cast<void*>(0x12345678)};
    CShaderRenderManagerUpdatePixelShaderView manager;
    memset(&manager, 0, sizeof(manager));
    manager.displayDevice1D4 = &device;
    manager.currentPixelShader1E4 = &shader;
    manager.shaderStateMask3D8 = 0xFFFFFFFF;

    manager.UpdatePixelShader();
    if (
        g_Calls != 1 ||
        g_Shader != shader.pixelShader04 ||
        manager.shaderStateMask3D8 != 0xFFFFFFFE)
    {
        return 1;
    }

    manager.currentPixelShader1E4 = 0;
    manager.shaderStateMask3D8 = 3;
    manager.UpdatePixelShader();
    if (g_Calls != 1 || manager.shaderStateMask3D8 != 2)
    {
        return 1;
    }

    printf("FABLETLC_UPDATE_PIXEL_SHADER_BEHAVIOR PASS\n");
    return 0;
}
