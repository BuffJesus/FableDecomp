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

void CShaderRenderManagerUpdateFogColourView::UpdateFogColour()
{
    CFogConstantLayoutView* layout = constantLayout04;
    if (layout->vectorCount3C != 0)
    {
        CFogFloatColour fogColour;
        const fable_i32 startRegister =
            layout->startRegister38;
        fogColour.red = fogColour1BC.red;
        fogColour.green = fogColour1BC.green;
        fogColour.blue = fogColour1BC.blue;
        fogColour.alpha = fogColour1BC.alpha;
        reinterpret_cast<FableSetFogConstantsCall>(
            displayDevice1D4->vtable[0x5E])(
                displayDevice1D4,
                startRegister,
                &fogColour,
                1);
    }
    shaderStateMask3D8 &= ~0x20000u;
}
