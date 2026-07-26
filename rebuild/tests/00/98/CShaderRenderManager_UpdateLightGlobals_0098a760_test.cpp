#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct FableLightGlobalsDevice;
typedef long (__stdcall *FableSetLightGlobalsCall)(
    FableLightGlobalsDevice* device,
    fable_i32 startRegister,
    const void* data,
    fable_u32 vectorCount);

struct FableLightGlobalsDevice
{
    void** vtable;
};

struct CLightGlobalLayoutView
{
    fable_u8 unknown00[0x60];
    fable_i32 startRegister60;
    fable_u32 vectorCount64;
};

struct CShaderRenderManagerUpdateLightGlobalsView
{
    fable_u8 unknown000[4];
    CLightGlobalLayoutView* lightLayout04;
    fable_u8 unknown008[0x18C];
    const void* lightGlobals194;
    fable_u8 unknown198[0x3C];
    FableLightGlobalsDevice* displayDevice1D4;
    fable_u8 unknown1D8[0x200];
    fable_u32 shaderStateMask3D8;

    void UpdateLightGlobals();
};

static int g_Calls = 0;
static fable_i32 g_StartRegister = 0;
static const void* g_Data = 0;
static fable_u32 g_VectorCount = 0;

static long __stdcall SetLightGlobalsCall(
    FableLightGlobalsDevice*,
    fable_i32 startRegister,
    const void* data,
    fable_u32 vectorCount)
{
    ++g_Calls;
    g_StartRegister = startRegister;
    g_Data = data;
    g_VectorCount = vectorCount;
    return 0;
}

int main()
{
    void* vtable[0x5F] = {0};
    vtable[0x5E] = reinterpret_cast<void*>(&SetLightGlobalsCall);
    FableLightGlobalsDevice device = {vtable};
    CLightGlobalLayoutView layout;
    CShaderRenderManagerUpdateLightGlobalsView manager;
    float constants[12] = {0};
    memset(&layout, 0, sizeof(layout));
    memset(&manager, 0, sizeof(manager));
    layout.startRegister60 = 12;
    layout.vectorCount64 = 3;
    manager.lightLayout04 = &layout;
    manager.lightGlobals194 = constants;
    manager.displayDevice1D4 = &device;
    manager.shaderStateMask3D8 = 0xFFFFFFFF;

    manager.UpdateLightGlobals();
    if (
        g_Calls != 1 ||
        g_StartRegister != 12 ||
        g_Data != constants ||
        g_VectorCount != 3 ||
        manager.shaderStateMask3D8 != 0xFFFFFBFF)
    {
        return 1;
    }

    layout.vectorCount64 = 0;
    manager.shaderStateMask3D8 = 0x400;
    manager.UpdateLightGlobals();
    if (g_Calls != 1 || manager.shaderStateMask3D8 != 0)
    {
        return 1;
    }

    printf("FABLETLC_UPDATE_LIGHT_GLOBALS_BEHAVIOR PASS\n");
    return 0;
}
