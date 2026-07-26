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
    virtual void Release(fable_i32 freeMemory);
    void* pixelShader04;
    fable_i32 referenceCount08;
};

struct CCountedPixelShaderHandle
{
    CCountedPixelShaderView* pointer;

    CCountedPixelShaderHandle& operator=(
        CCountedPixelShaderView* next)
    {
        CCountedPixelShaderView* shader = pointer;
        if (shader != 0)
        {
            --shader->referenceCount08;
            if (shader->referenceCount08 == 0)
            {
                shader->Release(1);
            }
        }
        pointer = next;
        return *this;
    }
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

void CShaderRenderManagerDisablePixelShaderView::
DisablePixelShaders()
{
    if (pixelShadersEnabled391)
    {
        reinterpret_cast<FableSetPixelShaderCall>(
            displayDevice1D4->vtable[0x6B])(
                displayDevice1D4,
                0);
        pixelShadersEnabled391 = 0;
        shaderStateMask3D8 &= ~3u;
        currentPixelShader1E4 =
            static_cast<CCountedPixelShaderView*>(0);
    }
}
