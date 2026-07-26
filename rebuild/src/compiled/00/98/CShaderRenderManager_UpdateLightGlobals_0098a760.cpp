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

void CShaderRenderManagerUpdateLightGlobalsView::UpdateLightGlobals()
{
    const fable_u32 vectorCount = lightLayout04->vectorCount64;
    if (vectorCount != 0)
    {
        reinterpret_cast<FableSetLightGlobalsCall>(
            displayDevice1D4->vtable[0x5E])(
                displayDevice1D4,
                lightLayout04->startRegister60,
                lightGlobals194,
                vectorCount);
    }
    shaderStateMask3D8 &= ~0x400u;
}
