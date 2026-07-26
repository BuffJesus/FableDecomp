#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct FablePixelShaderDevice;
typedef long (__stdcall *FableSetPixelConstantsCall)(
    FablePixelShaderDevice* device,
    fable_i32 startRegister,
    const void* data,
    fable_u32 vectorCount);

struct FablePixelShaderDevice
{
    void** vtable;
};

struct CPixelShaderConstantVector
{
    fable_u32 values[4];

    CPixelShaderConstantVector();
};

struct CPixelShaderConstantDataView
{
    fable_u8 unknown00[0x0C];
    fable_u32 constantCount0C;
    const fable_u32* constantIndices10;
};

struct CShaderRenderManagerUpdatePixelConstantsView
{
    fable_u8 unknown000[0x1D4];
    FablePixelShaderDevice* displayDevice1D4;
    fable_u8 unknown1D8[0x0C];
    CPixelShaderConstantDataView* pixelShaderData1E4;
    fable_u8 unknown1E8[0x1E4];
    const CPixelShaderConstantVector* pixelConstants3CC;
    fable_u8 unknown3D0[8];
    fable_u32 shaderStateMask3D8;

    void UpdatePixelShaderConstants();
};

static fable_i32 g_Calls;
static FablePixelShaderDevice* g_Device;
static fable_i32 g_StartRegister;
static const CPixelShaderConstantVector* g_Constants;
static fable_u32 g_ConstantCount;
static CPixelShaderConstantVector g_Observed[8];

static long __stdcall ObservePixelConstants(
    FablePixelShaderDevice* device,
    fable_i32 startRegister,
    const void* data,
    fable_u32 vectorCount)
{
    ++g_Calls;
    g_Device = device;
    g_StartRegister = startRegister;
    g_Constants =
        static_cast<const CPixelShaderConstantVector*>(data);
    g_ConstantCount = vectorCount;
    for (fable_u32 index = 0; index < vectorCount; ++index)
    {
        g_Observed[index] = g_Constants[index];
    }
    return 0;
}

static bool Equal(
    const CPixelShaderConstantVector& left,
    const CPixelShaderConstantVector& right)
{
    return left.values[0] == right.values[0] &&
        left.values[1] == right.values[1] &&
        left.values[2] == right.values[2] &&
        left.values[3] == right.values[3];
}

int main()
{
    void* vtable[0x6E] = {0};
    vtable[0x6D] =
        reinterpret_cast<void*>(&ObservePixelConstants);
    FablePixelShaderDevice device = {vtable};

    CPixelShaderConstantVector source[8];
    for (fable_u32 index = 0; index < 8; ++index)
    {
        source[index].values[0] = index * 10 + 1;
        source[index].values[1] = index * 10 + 2;
        source[index].values[2] = index * 10 + 3;
        source[index].values[3] = index * 10 + 4;
    }
    const fable_u32 indices[3] = {2, 0, 5};
    CPixelShaderConstantDataView shaderData = {};
    shaderData.constantCount0C = 3;
    shaderData.constantIndices10 = indices;
    CShaderRenderManagerUpdatePixelConstantsView manager;
    memset(&manager, 0, sizeof(manager));
    manager.displayDevice1D4 = &device;
    manager.pixelShaderData1E4 = &shaderData;
    manager.pixelConstants3CC = source;
    manager.shaderStateMask3D8 = 0xFFFFFFFF;

    manager.UpdatePixelShaderConstants();
    if (g_Calls != 1 ||
        g_Device != &device ||
        g_StartRegister != 0 ||
        g_ConstantCount != 3 ||
        !Equal(g_Observed[0], source[2]) ||
        !Equal(g_Observed[1], source[0]) ||
        !Equal(g_Observed[2], source[5]) ||
        manager.shaderStateMask3D8 != 0xFFFFFFFD)
    {
        printf(
            "FABLETLC_UPDATE_PIXEL_SHADER_CONSTANTS_BEHAVIOR "
            "FAIL code=1\n");
        return 1;
    }

    const CPixelShaderConstantVector* firstBuffer =
        g_Constants;
    source[2].values[0] = 999;
    manager.shaderStateMask3D8 = 2;
    manager.UpdatePixelShaderConstants();
    if (g_Calls != 2 ||
        g_Constants != firstBuffer ||
        g_Observed[0].values[0] != 999 ||
        manager.shaderStateMask3D8 != 0)
    {
        printf(
            "FABLETLC_UPDATE_PIXEL_SHADER_CONSTANTS_BEHAVIOR "
            "FAIL code=2\n");
        return 2;
    }

    shaderData.constantCount0C = 0;
    manager.shaderStateMask3D8 = 2;
    manager.UpdatePixelShaderConstants();
    if (g_Calls != 2 || manager.shaderStateMask3D8 != 0)
    {
        printf(
            "FABLETLC_UPDATE_PIXEL_SHADER_CONSTANTS_BEHAVIOR "
            "FAIL code=3\n");
        return 3;
    }

    manager.pixelShaderData1E4 = 0;
    manager.shaderStateMask3D8 = 2;
    manager.UpdatePixelShaderConstants();
    if (g_Calls != 2 || manager.shaderStateMask3D8 != 0)
    {
        printf(
            "FABLETLC_UPDATE_PIXEL_SHADER_CONSTANTS_BEHAVIOR "
            "FAIL code=4\n");
        return 4;
    }

    printf(
        "FABLETLC_UPDATE_PIXEL_SHADER_CONSTANTS_BEHAVIOR PASS\n");
    return 0;
}
