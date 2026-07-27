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

void CShaderRenderManagerUpdatePixelShaderView::UpdatePixelShader()
{
    if (currentPixelShader1E4 != 0)
    {
        reinterpret_cast<FableSetPixelShaderCall>(
            displayDevice1D4->vtable[0x6B])(
                displayDevice1D4,
                currentPixelShader1E4->pixelShader04);
    }
    shaderStateMask3D8 &= ~1u;
}
