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

CPixelShaderConstantVector::CPixelShaderConstantVector()
{
}

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

void CShaderRenderManagerUpdatePixelConstantsView::
UpdatePixelShaderConstants()
{
    CPixelShaderConstantDataView* shaderData =
        pixelShaderData1E4;
    if (shaderData == 0)
    {
        goto clearUpdateFlag;
    }
    {
        const fable_u32 constantCount =
            shaderData->constantCount0C;
        const fable_u32* constantIndices =
            shaderData->constantIndices10;
        static CPixelShaderConstantVector constants[8];
        for (fable_u32 index = 0;
             index < constantCount;
             ++index)
        {
            constants[index] =
                pixelConstants3CC[constantIndices[index]];
        }
        if (constantCount != 0)
        {
            reinterpret_cast<FableSetPixelConstantsCall>(
                displayDevice1D4->vtable[0x6D])(
                    displayDevice1D4,
                    0,
                    constants,
                    constantCount);
        }
    }
clearUpdateFlag:
    shaderStateMask3D8 &= ~2u;
}
