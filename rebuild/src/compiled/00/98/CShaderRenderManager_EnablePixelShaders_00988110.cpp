#include "rebuild_abi.h"

struct CDisplayManagerPixelShaderView
{
    fable_u32 GetPixelShaderVersion() const;
};

struct CDisplayEnginePixelShaderView
{
    fable_u8 unknown0000[0x3A3C];
    CDisplayManagerPixelShaderView* displayManager3A3C;
};

struct CShaderRenderManagerPixelShaderView
{
    fable_u8 unknown000[0x1D0];
    CDisplayEnginePixelShaderView* displayEngine1D0;
    fable_u8 unknown1D4[0x1BD];
    fable_u8 pixelShadersEnabled391;

    void EnablePixelShaders();
};

void CShaderRenderManagerPixelShaderView::EnablePixelShaders()
{
    if (displayEngine1D0
            ->displayManager3A3C
            ->GetPixelShaderVersion() >= 1000)
    {
        pixelShadersEnabled391 = 1;
    }
}
