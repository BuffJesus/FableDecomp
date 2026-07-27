#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct FableFogShaderDevice;
typedef long (__stdcall *FableSetFogConstantsCall)(
    FableFogShaderDevice* device,
    fable_i32 startRegister,
    const void* data,
    fable_u32 vectorCount);

struct FableFogShaderDevice
{
    void** vtable;
};

struct CFogConstantLayoutView
{
    fable_u8 unknown00[0x38];
    fable_i32 startRegister38;
    fable_u32 vectorCount3C;
};

struct CFogFloatColour
{
    float red;
    float green;
    float blue;
    float alpha;
};

struct CShaderRenderManagerUpdateFogColourView
{
    fable_u8 unknown000[4];
    CFogConstantLayoutView* constantLayout04;
    fable_u8 unknown008[0x1B4];
    CFogFloatColour fogColour1BC;
    fable_u8 unknown1CC[8];
    FableFogShaderDevice* displayDevice1D4;
    fable_u8 unknown1D8[0x200];
    fable_u32 shaderStateMask3D8;

    void UpdateFogColour();
};

static fable_i32 g_Calls;
static FableFogShaderDevice* g_Device;
static fable_i32 g_StartRegister;
static CFogFloatColour g_ObservedColour;
static fable_u32 g_VectorCount;

static long __stdcall ObserveFogConstants(
    FableFogShaderDevice* device,
    fable_i32 startRegister,
    const void* data,
    fable_u32 vectorCount)
{
    ++g_Calls;
    g_Device = device;
    g_StartRegister = startRegister;
    g_ObservedColour =
        *static_cast<const CFogFloatColour*>(data);
    g_VectorCount = vectorCount;
    return 0;
}

int main()
{
    void* vtable[0x5F] = {0};
    vtable[0x5E] =
        reinterpret_cast<void*>(&ObserveFogConstants);
    FableFogShaderDevice device = {vtable};
    CFogConstantLayoutView layout;
    CShaderRenderManagerUpdateFogColourView manager;
    memset(&layout, 0, sizeof(layout));
    memset(&manager, 0, sizeof(manager));

    layout.startRegister38 = 12;
    layout.vectorCount3C = 3;
    manager.constantLayout04 = &layout;
    manager.fogColour1BC.red = 0.25f;
    manager.fogColour1BC.green = 0.5f;
    manager.fogColour1BC.blue = 0.75f;
    manager.fogColour1BC.alpha = 1.0f;
    manager.displayDevice1D4 = &device;
    manager.shaderStateMask3D8 = 0xFFFFFFFF;

    manager.UpdateFogColour();
    if (g_Calls != 1 ||
        g_Device != &device ||
        g_StartRegister != 12 ||
        g_ObservedColour.red != 0.25f ||
        g_ObservedColour.green != 0.5f ||
        g_ObservedColour.blue != 0.75f ||
        g_ObservedColour.alpha != 1.0f ||
        g_VectorCount != 1 ||
        manager.shaderStateMask3D8 != 0xFFFDFFFF)
    {
        printf("FABLETLC_UPDATE_FOG_COLOUR_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    layout.vectorCount3C = 0;
    manager.shaderStateMask3D8 = 0x20000;
    manager.UpdateFogColour();
    if (g_Calls != 1 || manager.shaderStateMask3D8 != 0)
    {
        printf("FABLETLC_UPDATE_FOG_COLOUR_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_UPDATE_FOG_COLOUR_BEHAVIOR PASS\n");
    return 0;
}
