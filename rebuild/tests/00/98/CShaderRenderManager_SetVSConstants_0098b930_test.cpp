#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct FableShaderDevice;
typedef long (__stdcall *FableSetVSConstantsCall)(
    FableShaderDevice* device,
    fable_i32 startRegister,
    const void* data,
    fable_u32 vectorCount);

struct FableShaderDevice
{
    void** vtable;
};

struct CShaderRenderManagerSetVSConstantsView
{
    fable_u8 unknown000[0x1D4];
    FableShaderDevice* displayDevice1D4;

    void SetVSConstants(
        fable_i32 startRegister,
        const void* data,
        fable_u32 vectorCount);
};

static int g_Calls = 0;
static fable_i32 g_StartRegister = 0;
static const void* g_Data = 0;
static fable_u32 g_VectorCount = 0;

static long __stdcall SetVSConstantsCall(
    FableShaderDevice*,
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
    vtable[0x5E] = reinterpret_cast<void*>(&SetVSConstantsCall);
    FableShaderDevice device = {vtable};
    CShaderRenderManagerSetVSConstantsView manager;
    memset(&manager, 0, sizeof(manager));
    manager.displayDevice1D4 = &device;
    float constants[8] = {0};

    manager.SetVSConstants(4, constants, 0);
    if (g_Calls != 0)
    {
        return 1;
    }

    manager.SetVSConstants(4, constants, 2);
    if (
        g_Calls != 1 ||
        g_StartRegister != 4 ||
        g_Data != constants ||
        g_VectorCount != 2)
    {
        return 1;
    }

    printf("FABLETLC_SET_VS_CONSTANTS_BEHAVIOR PASS\n");
    return 0;
}
